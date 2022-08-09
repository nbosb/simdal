/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db/db.h"

#include <cmath>
#include <iostream>

#include "simdal/utils.h"

namespace simdal {

db::db(const std::string& path)
    : path_(path),
      csv_queue_status_(nullptr),
      db_worker_thread_(nullptr),
      db_worker_thread_stop_(true),
      db_opened_(false),
      max_queue_size_(init_max_queue_size),
      queue_relieve_level_(init_queue_relieve_level) {}

db::~db() {
    stop_db_worker_thread();
    stop_log_queue_status();
}

bool db::open_db() {
    if (file_exists(path_)) {
        LOG_DBG("database %s, already exists!", path_.c_str());
    }

    bool open_succeeded = false;
    if (!db_opened_ && open()) {
        db_opened_ = true;

        if (init()) {
            if (db_worker_thread_ != nullptr) {
                LOG_DBG("db_worker_thread_ is no nullptr");
                return false;
            }

            // start worker thread
            db_worker_thread_stop_ = false;
            db_worker_thread_ = std::unique_ptr<std::thread>(
                new std::thread(&db::db_worker_thread_func, this));
            open_succeeded = true;
        } else {
            close_db();
        }
    }

    return open_succeeded;
}

bool db::close_db() {
    if (db_opened_) {
        {
            std::unique_lock<std::mutex> lock(entry_queue_mtx_);
            if (!entry_queue_.empty() && !db_worker_thread_stop_) {
                cv_from_worker_.wait(lock, [this] {
                    return (entry_queue_.empty() || db_worker_thread_stop_);
                });  // wait until worker is done
            }
        }
        stop_db_worker_thread();
        if (close()) {
            db_opened_ = false;
            return true;
        }
    }

    return false;
}

void db::db_worker_thread_func() {
    LOG_DBG("db worker started");

    entry_base* entry = nullptr;
    unsigned int queue_size = 0;
    while (true) {
        {
            std::unique_lock<std::mutex> lock(entry_queue_mtx_);
            cv_to_worker_.wait(lock, [this] {
                return (!entry_queue_.empty() || db_worker_thread_stop_);
            });

            if (db_worker_thread_stop_) {
                cv_from_worker_.notify_all();  // used if close_db() is called
                break;
            }

            entry = entry_queue_.back();
            entry_queue_.pop_back();
            queue_size = entry_queue_.size();
        }

        store_entry(entry);
        entry->free();

        if (csv_queue_status_) {
            csv_queue_status_->write(get_time_now_str() + ";" +
                                     std::to_string(queue_size));
        }

        cv_from_worker_
            .notify_all();  // used if close_db() or relieve_queue() is called
    }

    LOG_DBG("db worker finished");
}

void db::stop_db_worker_thread() {
    if (!db_worker_thread_stop_) {
        if (db_worker_thread_ == nullptr) {
            LOG_DBG("worker thread is running but pointer is nullptr");
        }

        relieve_queue(0);

        db_worker_thread_stop_ = true;
        cv_to_worker_.notify_one();

        db_worker_thread_->join();
        db_worker_thread_ = nullptr;
        LOG_DBG("db worker thread joined");
    }
}

bool db::store(entry_base* entry) {
    if (!db_opened_ || db_worker_thread_stop_) {
        LOG_DBG("db not opened or worker thread not running!");
        entry->free();  // free entry because it cannot be stored
        return false;
    }

    unsigned int queue_size = 0;
    {
        std::lock_guard<std::mutex> lock(entry_queue_mtx_);
        entry_queue_.push_front(entry);
        queue_size = entry_queue_.size();
    }
    cv_to_worker_.notify_one();

    if (queue_size > max_queue_size_) {
        relieve_queue(queue_relieve_level_);
    }
    return true;
}

std::string db::get_path() const { return path_; }

std::string db::get_type() const { return "db"; }

bool db::is_open() const { return db_opened_; }

bool db::relieve_queue(const unsigned int level) {
    if (!db_opened_ || db_worker_thread_stop_) {
        return false;
    }
    {
        std::unique_lock<std::mutex> lock(entry_queue_mtx_);

        if (entry_queue_.size() <= level) {
            return true;
        }

        cv_from_worker_.wait(lock, [this, level] {
            return (entry_queue_.size() <= level || db_worker_thread_stop_);
        });
    }
    LOG_DBG("relieved queue to %d entries", level);
    return true;
}

void db::set_max_queue_size(unsigned int size) {
    max_queue_size_ = size;

    if (queue_relieve_level_ >= max_queue_size_) {
        queue_relieve_level_ = std::ceil(0.8 * max_queue_size_);
    }
}

void db::set_queue_relieve_level(unsigned int level) {
    queue_relieve_level_ = level;

    if (max_queue_size_ <= queue_relieve_level_) {
        max_queue_size_ = std::ceil(queue_relieve_level_ / 0.8);
    }
}

void db::start_log_queue_status(const std::string& path) {
    if (csv_queue_status_) {
        return;
    }

    csv_queue_status_ = std::unique_ptr<csv_file>(new csv_file(path));
    csv_queue_status_->open();
    csv_queue_status_->write("timestamp;queue_size");
}

void db::stop_log_queue_status() {
    if (!csv_queue_status_) {
        return;
    }
    csv_queue_status_->close();
    csv_queue_status_.reset();
}

bool db::store_entry(entry_base* entry) {
    if (dynamic_cast<entry_quantum*>(entry) != nullptr) {
        return store_entry(static_cast<entry_quantum*>(entry));
    }
    if (dynamic_cast<entry_sc_sched*>(entry) != nullptr) {
        return store_entry(static_cast<entry_sc_sched*>(entry));
    }
    if (dynamic_cast<entry_event*>(entry) != nullptr) {
        return store_entry(static_cast<entry_event*>(entry));
    }
    if (dynamic_cast<entry_cout*>(entry) != nullptr) {
        return store_entry(static_cast<entry_cout*>(entry));
    }
    if (dynamic_cast<entry_bp_hit*>(entry) != nullptr) {
        return store_entry(static_cast<entry_bp_hit*>(entry));
    }
    if (dynamic_cast<entry_task_switch*>(entry) != nullptr) {
        return store_entry(static_cast<entry_task_switch*>(entry));
    }
    if (dynamic_cast<entry_transaction*>(entry) != nullptr) {
        return store_entry(static_cast<entry_transaction*>(entry));
    }
    if (dynamic_cast<entry_wait_event*>(entry) != nullptr) {
        return store_entry(static_cast<entry_wait_event*>(entry));
    }
    if (dynamic_cast<entry_call_stack*>(entry) != nullptr) {
        return store_entry(static_cast<entry_call_stack*>(entry));
    }
    if (dynamic_cast<entry_interrupt*>(entry) != nullptr) {
        return store_entry(static_cast<entry_interrupt*>(entry));
    }
    if (dynamic_cast<entry_exception*>(entry) != nullptr) {
        return store_entry(static_cast<entry_exception*>(entry));
    }
    if (dynamic_cast<entry_bp_hit_desc*>(entry) != nullptr) {
        return store_entry(static_cast<entry_bp_hit_desc*>(entry));
    }
    if (dynamic_cast<entry_target*>(entry) != nullptr) {
        return store_entry(static_cast<entry_target*>(entry));
    }
    if (dynamic_cast<entry_sniffer*>(entry) != nullptr) {
        return store_entry(static_cast<entry_sniffer*>(entry));
    }
    if (dynamic_cast<entry_vcml_payload_irq*>(entry) != nullptr) {
        return store_entry(static_cast<entry_vcml_payload_irq*>(entry));
    }
    if (dynamic_cast<entry_vcml_payload_pci*>(entry) != nullptr) {
        return store_entry(static_cast<entry_vcml_payload_pci*>(entry));
    }
    if (dynamic_cast<entry_vcml_payload_sd_cmd*>(entry) != nullptr) {
        return store_entry(static_cast<entry_vcml_payload_sd_cmd*>(entry));
    }
    if (dynamic_cast<entry_vcml_payload_sd_data*>(entry) != nullptr) {
        return store_entry(static_cast<entry_vcml_payload_sd_data*>(entry));
    }
    if (dynamic_cast<entry_vcml_payload_spi*>(entry) != nullptr) {
        return store_entry(static_cast<entry_vcml_payload_spi*>(entry));
    }
    if (dynamic_cast<entry_vcml_payload_tlm*>(entry) != nullptr) {
        return store_entry(static_cast<entry_vcml_payload_tlm*>(entry));
    }
    if (dynamic_cast<entry_vcml_payload_virtio*>(entry) != nullptr) {
        return store_entry(static_cast<entry_vcml_payload_virtio*>(entry));
    }
    if (dynamic_cast<entry_sc_proc*>(entry) != nullptr) {
        return store_entry(static_cast<entry_sc_proc*>(entry));
    }

    return false;
}

}  // namespace simdal
