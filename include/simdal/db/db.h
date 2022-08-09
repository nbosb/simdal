/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_DB_H_
#define SIMDAL_DB_DB_H_

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "simdal/csv/csv_file.h"
#include "simdal/db_entry/entry.h"
#include "simdal/db_entry/entry_bp_hit.h"
#include "simdal/db_entry/entry_bp_hit_desc.h"
#include "simdal/db_entry/entry_call_stack.h"
#include "simdal/db_entry/entry_cout.h"
#include "simdal/db_entry/entry_event.h"
#include "simdal/db_entry/entry_exception.h"
#include "simdal/db_entry/entry_interrupt.h"
#include "simdal/db_entry/entry_quantum.h"
#include "simdal/db_entry/entry_sc_proc.h"
#include "simdal/db_entry/entry_sc_sched.h"
#include "simdal/db_entry/entry_sniffer.h"
#include "simdal/db_entry/entry_target.h"
#include "simdal/db_entry/entry_task_switch.h"
#include "simdal/db_entry/entry_transaction.h"
#include "simdal/db_entry/entry_vcml_payload_irq.h"
#include "simdal/db_entry/entry_vcml_payload_pci.h"
#include "simdal/db_entry/entry_vcml_payload_sd_cmd.h"
#include "simdal/db_entry/entry_vcml_payload_sd_data.h"
#include "simdal/db_entry/entry_vcml_payload_spi.h"
#include "simdal/db_entry/entry_vcml_payload_tlm.h"
#include "simdal/db_entry/entry_vcml_payload_virtio.h"
#include "simdal/db_entry/entry_wait_event.h"
#include "simdal/mem_pool.h"

namespace simdal {
class db {
  public:
    typedef std::shared_ptr<db> ptr;

    explicit db(const std::string& path);
    db() = delete;
    db(const db& db) = delete;
    virtual ~db();

    bool open_db();
    bool close_db();

    bool store(entry_base* entry);
    virtual std::string get_path() const;
    virtual std::string get_type() const;
    bool is_open() const;
    bool relieve_queue(unsigned int level = 0);
    void set_max_queue_size(unsigned int size);
    void set_queue_relieve_level(unsigned int level);
    void start_log_queue_status(const std::string& path);
    void stop_log_queue_status();

  protected:
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool init() = 0;
    virtual bool store_entry(entry_base* entry);
    virtual bool store_entry(entry_bp_hit* entry) = 0;
    virtual bool store_entry(entry_bp_hit_desc* entry) = 0;
    virtual bool store_entry(entry_call_stack* entry) = 0;
    virtual bool store_entry(entry_cout* entry) = 0;
    virtual bool store_entry(entry_event* entry) = 0;
    virtual bool store_entry(entry_exception* entry) = 0;
    virtual bool store_entry(entry_interrupt* entry) = 0;
    virtual bool store_entry(entry_quantum* entry) = 0;
    virtual bool store_entry(entry_sc_proc* entry) = 0;
    virtual bool store_entry(entry_sc_sched* entry) = 0;
    virtual bool store_entry(entry_sniffer* entry) = 0;
    virtual bool store_entry(entry_task_switch* entry) = 0;
    virtual bool store_entry(entry_target* entry) = 0;
    virtual bool store_entry(entry_transaction* entry) = 0;
    virtual bool store_entry(entry_vcml_payload_irq* entry) = 0;
    virtual bool store_entry(entry_vcml_payload_pci* entry) = 0;
    virtual bool store_entry(entry_vcml_payload_sd_cmd* entry) = 0;
    virtual bool store_entry(entry_vcml_payload_sd_data* entry) = 0;
    virtual bool store_entry(entry_vcml_payload_spi* entry) = 0;
    virtual bool store_entry(entry_vcml_payload_tlm* entry) = 0;
    virtual bool store_entry(entry_vcml_payload_virtio* entry) = 0;
    virtual bool store_entry(entry_wait_event* entry) = 0;
    void stop_db_worker_thread();

    std::string path_;

  private:
    void db_worker_thread_func();

    const unsigned int init_max_queue_size = 100;
    const unsigned int init_queue_relieve_level = 80;

    std::unique_ptr<csv_file> csv_queue_status_;
    std::unique_ptr<std::thread> db_worker_thread_;
    bool db_worker_thread_stop_;
    bool db_opened_;
    std::deque<entry_base*> entry_queue_;
    std::mutex entry_queue_mtx_;
    std::condition_variable cv_to_worker_;    // used to start / continue worker
    std::condition_variable cv_from_worker_;  // used by the worker to signal
                                              // that work has been done
    unsigned int max_queue_size_;
    unsigned int queue_relieve_level_;
};

}  // namespace simdal
#endif  // SIMDAL_DB_DB_H_
