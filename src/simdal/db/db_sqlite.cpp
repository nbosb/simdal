/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db/db_sqlite.h"

#include <iostream>

#include "simdal/utils.h"

namespace simdal {

const std::string db_sqlite::sc_cmd_beg_trans_ = "BEGIN IMMEDIATE TRANSACTION;";
const std::string db_sqlite::sc_cmd_end_trans_ = "END TRANSACTION;";
const std::string db_sqlite::sc_cmd_bp_hit_ =
    "INSERT INTO bp_hit (timestamp,timestamp_rt,target_id,bp_id) VALUES (?1, "
    "?2, (SELECT id FROM target WHERE name=?3), (SELECT id FROM bp_hit_desc "
    "WHERE desc=?4));";
const std::string db_sqlite::sc_cmd_bp_hit_desc_ =
    "INSERT INTO bp_hit_desc (desc) VALUES (?1);";
const std::string db_sqlite::sc_cmd_call_stack_ =
    "INSERT INTO call_stack (timestamp, timestamp_rt, target_id) VALUES (?1, "
    "?2, (SELECT id FROM target WHERE name=?3));";
const std::string db_sqlite::sc_cmd_call_stack_frame_ =
    "INSERT INTO call_stack_frame (call_stack_id, level, pc, fp, symbol_id) "
    "VALUES (?1, ?2, ?3, ?4, (SELECT id FROM symbol WHERE name=?5));";
const std::string db_sqlite::sc_cmd_exception_ =
    "INSERT INTO exception "
    "(exc_class,timestamp_entry,timestamp_entry_rt,timestamp_exit,timestamp_"
    "exit_rt,target_id) VALUES (?1, ?2, ?3, ?4, ?5, (SELECT id FROM target "
    "WHERE name=?6));";
const std::string db_sqlite::sc_cmd_interrupt_ =
    "INSERT INTO interrupt "
    "(number,timestamp_entry,timestamp_entry_rt,timestamp_exit,timestamp_exit_"
    "rt,target_id) VALUES (?1, ?2, ?3, ?4, ?5, (SELECT id FROM target WHERE "
    "name=?6));";
const std::string db_sqlite::sc_cmd_quantum_ =
    "INSERT INTO quantum (length,timestamp,timestamp_rt,target_id) VALUES (?1, "
    "?2, ?3, (SELECT id FROM target WHERE name=?4));";
const std::string db_sqlite::sc_cmd_sc_proc_ =
    "INSERT INTO sc_proc (name,type) VALUES (?1, ?2);";
const std::string db_sqlite::sc_cmd_sc_sched_ =
    "INSERT INTO sc_sched "
    "(timestamp_begin,timestamp_begin_rt,timestamp_end,timestamp_end_rt,sc_"
    "proc_id) VALUES(?1, ?2, ?3, ?4, (SELECT id FROM sc_proc WHERE name=?5));";
const std::string db_sqlite::sc_cmd_sniffer_ =
    "INSERT INTO sniffer (name) VALUES (?1);";
const std::string db_sqlite::sc_cmd_target_ =
    "INSERT INTO target (name) VALUES (?1);";
const std::string db_sqlite::sc_cmd_task_switch_ =
    "INSERT INTO task_switch "
    "(pid_prev,name_prev,pid_next,name_next,timestamp,timestamp_rt,target_id) "
    "VALUES (?1, ?2, ?3, ?4, ?5, ?6, (SELECT id FROM target WHERE name=?7));";
const std::string db_sqlite::sc_cmd_transaction_ =
    "INSERT INTO trans "
    "(timestamp_fw,timestamp_fw_rt,timestamp_bw,timestamp_bw_rt,address,length,"
    "write,sniffer_id,dmi_req) VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, (SELECT id "
    "FROM sniffer WHERE name=?8), ?9);";
const std::string db_sqlite::sc_cmd_cout_ =
    "INSERT INTO cout (timestamp,timestamp_rt,text) VALUES (?1, ?2, ?3);";
const std::string db_sqlite::sc_cmd_wait_event_ =
    "INSERT INTO wait_event "
    "(timestamp_begin,timestamp_begin_rt,timestamp_end,timestamp_end_rt,target_"
    "id) VALUES (?1, ?2, ?3, ?4, (SELECT id FROM target WHERE name=?5));";
const std::string db_sqlite::sc_cmd_symbol_ =
    "INSERT OR IGNORE INTO symbol(name, address, size) VALUES (?1, ?2, ?3);";
const std::string db_sqlite::sc_cmd_event_ =
    "INSERT INTO event (timestamp, timestamp_rt, delay, event, module_id) "
    "VALUES (?1, ?2, ?3, ?4, (SELECT id FROM module WHERE name=?5));";
const std::string db_sqlite::sc_cmd_module_ =
    "INSERT OR IGNORE INTO module(name) VALUES (?1);";
const std::string db_sqlite::sc_cmd_vcml_payload_ =
    "INSERT INTO vcml_payload (timestamp_fw, timestamp_fw_rt, timestamp_bw, "
    "timestamp_bw_rt, port, module_id) VALUES (?1, ?2, ?3, ?4, ?5, (SELECT id "
    "FROM module WHERE name=?6));";
const std::string db_sqlite::sc_cmd_vcml_payload_irq_ =
    "INSERT INTO vcml_payload_irq (vcml_payload_id, active) VALUES "
    "(?1, ?2);";
const std::string db_sqlite::sc_cmd_vcml_payload_pci_ =
    "INSERT INTO vcml_payload_pci (vcml_payload_id, command, response, "
    "address_space, address, data, size, debug) VALUES (?1, ?2, ?3, ?4, ?5, "
    "?6, ?7, ?8);";
const std::string db_sqlite::sc_cmd_vcml_payload_sd_cmd_ =
    "INSERT INTO vcml_payload_sd_cmd (vcml_payload_id, opcode, arg, crc, resp, "
    "resp_len, appcmd, spi, status) VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, "
    "?9);";
const std::string db_sqlite::sc_cmd_vcml_payload_sd_data_ =
    "INSERT INTO vcml_payload_sd_data (vcml_payload_id, mode, data, status) "
    "VALUES (?1, ?2, ?3, ?4);";
const std::string db_sqlite::sc_cmd_vcml_payload_spi_ =
    "INSERT INTO vcml_payload_spi (vcml_payload_id, mosi, miso) VALUES (?1, "
    "?2, ?3);";
const std::string db_sqlite::sc_cmd_vcml_payload_tlm_ =
    "INSERT INTO vcml_payload_tlm (vcml_payload_id, address, length, write, "
    "dmi) VALUES (?1, ?2, ?3, ?4, ?5);";
const std::string db_sqlite::sc_cmd_vcml_payload_virtio_ =
    "INSERT INTO vcml_payload_virtio (vcml_payload_id) VALUES (?1);";

db_sqlite::db_sqlite(const std::string& path)
    : db(path),
      db_handle_(nullptr),
      transaction_size_(0),
      max_transaction_size_(10000),
      within_transaction_(false),
      use_transactions_(true),
      stmt_beg_trans_(nullptr),
      stmt_end_trans_(nullptr),
      stmt_bp_hit_(nullptr),
      stmt_bp_hit_desc_(nullptr),
      stmt_cout_(nullptr),
      stmt_event_(nullptr),
      stmt_exception_(nullptr),
      stmt_interrupt_(nullptr),
      stmt_module_(nullptr),
      stmt_quantum_(nullptr),
      stmt_sc_proc_(nullptr),
      stmt_sc_sched_(nullptr),
      stmt_sniffer_(nullptr),
      stmt_target_(nullptr),
      stmt_task_switch_(nullptr),
      stmt_transaction_(nullptr),
      stmt_vcml_payload_irq_(nullptr),
      stmt_vcml_payload_pci_(nullptr),
      stmt_vcml_payload_sd_cmd_(nullptr),
      stmt_vcml_payload_sd_data_(nullptr),
      stmt_vcml_payload_spi_(nullptr),
      stmt_vcml_payload_tlm_(nullptr),
      stmt_vcml_payload_virtio_(nullptr),
      stmt_wait_event_(nullptr) {}

db_sqlite::ptr db_sqlite::create(const std::string& path) {
    return std::make_shared<db_sqlite>(path);
}

db_sqlite::~db_sqlite() {
    stop_db_worker_thread();
    if (is_open()) {
        db_sqlite::close();
    }
}

bool db_sqlite::open() {
    int ret = sqlite3_open(path_.c_str(), &db_handle_);

    if (ret != SQLITE_OK) {
        LOG_DBG("failed to open database '%s': %s", path_.c_str(),
                sqlite3_errmsg(db_handle_));
        return false;
    }

    return true;
}

bool db_sqlite::close() {
    end_transaction();
    finalize_statements();

    int ret = sqlite3_close(db_handle_);

    if (ret != SQLITE_OK) {
        LOG_DBG("failed to close database '%s': %s", path_.c_str(),
                sqlite3_errmsg(db_handle_));
        return false;
    }

    return true;
}

bool db_sqlite::init() {
    exec(
        "CREATE TABLE task_switch(id INTEGER PRIMARY KEY, pid_prev INTEGER, "
        "name_prev STRING, pid_next INTEGER, name_next STRING, timestamp "
        "BIGINT, timestamp_rt BIGINT, target_id INTEGER);");
    exec("CREATE TABLE target(id INTEGER PRIMARY KEY, name STRING);");
    exec(
        "CREATE TABLE interrupt(id INTEGER PRIMARY KEY, number INTEGER, "
        "timestamp_entry BIGINT, timestamp_entry_rt BIGINT, timestamp_exit "
        "BIGINT, timestamp_exit_rt BIGINT, target_id INTEGER);");
    exec(
        "CREATE TABLE exception(id INTEGER PRIMARY KEY, exc_class INTEGER, "
        "timestamp_entry BIGINT, timestamp_entry_rt BIGINT, timestamp_exit "
        "BIGINT, timestamp_exit_rt BIGINT, target_id INTEGER);");
    exec(
        "CREATE TABLE bp_hit(id INTEGER PRIMARY KEY, bp_id INTEGER, timestamp "
        "BIGINT, timestamp_rt BIGINT, target_id INTEGER);");
    exec("CREATE TABLE bp_hit_desc(id INTEGER PRIMARY KEY, desc STRING);");
    exec("CREATE TABLE sniffer(id INTEGER PRIMARY KEY, name STRING);");
    exec(
        "CREATE TABLE trans(id INTEGER PRIMARY KEY, timestamp_fw BIGINT, "
        "timestamp_fw_rt BIGINT, timestamp_bw BIGINT, timestamp_bw_rt BIGINT, "
        "address INTEGER, length INTEGER, write BOOLEAN, dmi_req BOOLEAN, "
        "sniffer_id INTEGER);");
    exec(
        "CREATE TABLE quantum(id INTEGER PRIMARY KEY, length INTEGER, "
        "timestamp BIGINT, timestamp_rt BIGINT, target_id INTEGER);");
    exec(
        "CREATE TABLE cout(id INTEGER PRIMARY KEY, timestamp BIGINT, "
        "timestamp_rt BIGINT, text STRING);");
    exec(
        "CREATE TABLE wait_event(id INTEGER PRIMARY KEY, timestamp_begin "
        "BIGINT, timestamp_begin_rt BIGINT, timestamp_end BIGINT, "
        "timestamp_end_rt BIGINT, target_id INTEGER);");
    exec(
        "CREATE TABLE call_stack(id INTEGER PRIMARY KEY, timestamp BIGINT, "
        "timestamp_rt BIGINT, target_id INTEGER);");
    exec(
        "CREATE TABLE call_stack_frame(id INTEGER PRIMARY KEY, call_stack_id "
        "INTEGER, level INTEGER, pc INTEGER, fp INTEGER, symbol_id INTEGER);");
    exec(
        "CREATE TABLE symbol(id INTEGER PRIMARY KEY, name STRING, address "
        "INTEGER, size INTEGER, UNIQUE(address));");
    exec(
        "CREATE TABLE event(id INTEGER PRIMARY KEY, timestamp INTEGER, "
        "timestamp_rt INTEGER, delay INTEGER, event STRING, module_id "
        "INTEGER);");
    exec(
        "CREATE TABLE module(id INTEGER PRIMARY KEY, name STRING, "
        "UNIQUE(name));");
    exec(
        "CREATE TABLE vcml_payload(id INTEGER PRIMARY KEY, timestamp_fw "
        "INTEGER, timestamp_fw_rt INTEGER, timestamp_bw INTEGER, "
        "timestamp_bw_rt INTEGER, port STRING, module_id INTEGER);");
    exec(
        "CREATE TABLE vcml_payload_irq(id INTEGER PRIMARY KEY, vcml_payload_id "
        "INTEGER, active BOOLEAN);");
    exec(
        "CREATE TABLE vcml_payload_pci(id INTEGER PRIMARY KEY, vcml_payload_id "
        "INTEGER, command INTEGER, response INTEGER, address_space INTEGER, "
        "address INTEGER, data INTEGER, size INTEGER, debug BOOLEAN);");
    exec(
        "CREATE TABLE vcml_payload_sd_cmd(id INTEGER PRIMARY KEY, "
        "vcml_payload_id INTEGER, opcode INTEGER, arg INTEGER, crc INTEGER, "
        "resp BLOB, resp_len INTEGER, appcmd BOOLEAN, spi BOOLEAN, status "
        "INTEGER);");
    exec(
        "CREATE TABLE vcml_payload_sd_data(id INTEGER PRIMARY KEY, "
        "vcml_payload_id INTEGER, mode INTEGER, data INTEGER, status "
        "INTEGER);");
    exec(
        "CREATE TABLE vcml_payload_spi(id INTEGER PRIMARY KEY, vcml_payload_id "
        "INTEGER, mosi INTEGER, miso INTEGER);");
    exec(
        "CREATE TABLE vcml_payload_tlm(id INTEGER PRIMARY KEY, vcml_payload_id "
        "INTEGER, address INTEGER, length INTEGER, write BOOLEAN, dmi "
        "BOOLEAN);");
    exec(
        "CREATE TABLE vcml_payload_virtio(id INTEGER PRIMARY KEY, "
        "vcml_payload_id INTEGER);");
    exec(
        "CREATE TABLE sc_sched(id INTEGER PRIMARY KEY, timestamp_begin "
        "INTEGER, timestamp_begin_rt INTGER, timestamp_end INTEGER, "
        "timestamp_end_rt INTEGER, sc_proc_id INTEGER);");
    exec(
        "CREATE TABLE sc_proc(id INTEGER PRIMARY KEY, name STRING, type "
        "INTEGER, UNIQUE(name));");

    exec("CREATE UNIQUE INDEX idx_targets_name ON target (name);");
    exec("CREATE UNIQUE INDEX idx_bp_hit_desc_desc ON bp_hit_desc (desc);");
    exec("CREATE UNIQUE INDEX idx_module ON module (name);");
    exec("CREATE UNIQUE INDEX idx_sniffer_name ON sniffer (name);");
    exec("CREATE UNIQUE INDEX idx_symbol ON symbol (name);");
    exec("CREATE UNIQUE INDEX idx_sc_proc ON symbol (name);");

    prepare_statements();

    start_transaction();
    return true;
}

bool db_sqlite::prepare_statements() {
    bool success = true;

    success = success && prep_stmt(&sc_cmd_beg_trans_[0], &stmt_beg_trans_);
    success = success && prep_stmt(&sc_cmd_end_trans_[0], &stmt_end_trans_);
    success = success && prep_stmt(&sc_cmd_bp_hit_[0], &stmt_bp_hit_);
    success = success && prep_stmt(&sc_cmd_bp_hit_desc_[0], &stmt_bp_hit_desc_);
    success = success && prep_stmt(&sc_cmd_call_stack_[0], &stmt_call_stack_);
    success = success &&
              prep_stmt(&sc_cmd_call_stack_frame_[0], &stmt_call_stack_frame_);
    success = success && prep_stmt(&sc_cmd_event_[0], &stmt_event_);
    success = success && prep_stmt(&sc_cmd_cout_[0], &stmt_cout_);
    success = success && prep_stmt(&sc_cmd_exception_[0], &stmt_exception_);
    success = success && prep_stmt(&sc_cmd_interrupt_[0], &stmt_interrupt_);
    success = success && prep_stmt(&sc_cmd_module_[0], &stmt_module_);
    success = success && prep_stmt(&sc_cmd_quantum_[0], &stmt_quantum_);
    success = success && prep_stmt(&sc_cmd_sc_proc_[0], &stmt_sc_proc_);
    success = success && prep_stmt(&sc_cmd_sc_sched_[0], &stmt_sc_sched_);
    success = success && prep_stmt(&sc_cmd_sniffer_[0], &stmt_sniffer_);
    success = success && prep_stmt(&sc_cmd_symbol_[0], &stmt_symbol_);
    success = success && prep_stmt(&sc_cmd_target_[0], &stmt_target_);
    success = success && prep_stmt(&sc_cmd_task_switch_[0], &stmt_task_switch_);
    success = success && prep_stmt(&sc_cmd_transaction_[0], &stmt_transaction_);
    success =
        success && prep_stmt(&sc_cmd_vcml_payload_[0], &stmt_vcml_payload_);
    success = success &&
              prep_stmt(&sc_cmd_vcml_payload_irq_[0], &stmt_vcml_payload_irq_);
    success = success &&
              prep_stmt(&sc_cmd_vcml_payload_pci_[0], &stmt_vcml_payload_pci_);
    success = success && prep_stmt(&sc_cmd_vcml_payload_sd_cmd_[0],
                                   &stmt_vcml_payload_sd_cmd_);
    success = success && prep_stmt(&sc_cmd_vcml_payload_sd_data_[0],
                                   &stmt_vcml_payload_sd_data_);
    success = success &&
              prep_stmt(&sc_cmd_vcml_payload_spi_[0], &stmt_vcml_payload_spi_);
    success = success &&
              prep_stmt(&sc_cmd_vcml_payload_tlm_[0], &stmt_vcml_payload_tlm_);
    success = success && prep_stmt(&sc_cmd_vcml_payload_virtio_[0],
                                   &stmt_vcml_payload_virtio_);
    success = success && prep_stmt(&sc_cmd_wait_event_[0], &stmt_wait_event_);

    return success;
}

bool db_sqlite::finalize_statements() {
    bool success = true;

    success = fin_stmt(stmt_beg_trans_) && success;
    success = fin_stmt(stmt_end_trans_) && success;
    success = fin_stmt(stmt_bp_hit_) && success;
    success = fin_stmt(stmt_bp_hit_desc_) && success;
    success = fin_stmt(stmt_call_stack_) && success;
    success = fin_stmt(stmt_call_stack_frame_) && success;
    success = fin_stmt(stmt_cout_) && success;
    success = fin_stmt(stmt_event_) && success;
    success = fin_stmt(stmt_exception_) && success;
    success = fin_stmt(stmt_interrupt_) && success;
    success = fin_stmt(stmt_module_) && success;
    success = fin_stmt(stmt_quantum_) && success;
    success = fin_stmt(stmt_sc_proc_) && success;
    success = fin_stmt(stmt_sc_sched_) && success;
    success = fin_stmt(stmt_sniffer_) && success;
    success = fin_stmt(stmt_symbol_) && success;
    success = fin_stmt(stmt_target_) && success;
    success = fin_stmt(stmt_task_switch_) && success;
    success = fin_stmt(stmt_transaction_) && success;
    success = fin_stmt(stmt_vcml_payload_) && success;
    success = fin_stmt(stmt_vcml_payload_irq_) && success;
    success = fin_stmt(stmt_vcml_payload_pci_) && success;
    success = fin_stmt(stmt_vcml_payload_sd_cmd_) && success;
    success = fin_stmt(stmt_vcml_payload_sd_data_) && success;
    success = fin_stmt(stmt_vcml_payload_spi_) && success;
    success = fin_stmt(stmt_vcml_payload_tlm_) && success;
    success = fin_stmt(stmt_vcml_payload_virtio_) && success;
    success = fin_stmt(stmt_wait_event_) && success;

    stmt_beg_trans_ = nullptr;
    stmt_end_trans_ = nullptr;
    stmt_bp_hit_ = nullptr;
    stmt_bp_hit_desc_ = nullptr;
    stmt_call_stack_ = nullptr;
    stmt_call_stack_frame_ = nullptr;
    stmt_cout_ = nullptr;
    stmt_event_ = nullptr;
    stmt_exception_ = nullptr;
    stmt_interrupt_ = nullptr;
    stmt_quantum_ = nullptr;
    stmt_sc_proc_ = nullptr;
    stmt_sc_sched_ = nullptr;
    stmt_sniffer_ = nullptr;
    stmt_symbol_ = nullptr;
    stmt_target_ = nullptr;
    stmt_task_switch_ = nullptr;
    stmt_transaction_ = nullptr;
    stmt_vcml_payload_ = nullptr;
    stmt_vcml_payload_irq_ = nullptr;
    stmt_vcml_payload_pci_ = nullptr;
    stmt_vcml_payload_sd_cmd_ = nullptr;
    stmt_vcml_payload_sd_data_ = nullptr;
    stmt_vcml_payload_spi_ = nullptr;
    stmt_vcml_payload_tlm_ = nullptr;
    stmt_vcml_payload_virtio_ = nullptr;
    stmt_wait_event_ = nullptr;

    return success;
}

bool db_sqlite::prep_stmt(const char* cmd, sqlite3_stmt** stmt) {
    int ret = sqlite3_prepare_v2(db_handle_, cmd, -1, stmt, nullptr);
    if (ret != SQLITE_OK || stmt == nullptr) {
        LOG_DBG("failed to prepare statement '%s': %s", cmd,
                sqlite3_errmsg(db_handle_));
        return false;
    }
    return true;
}

bool db_sqlite::fin_stmt(sqlite3_stmt* stmt) {
    int ret = sqlite3_finalize(stmt);
    if (ret != SQLITE_OK) {
        LOG_DBG("failed to finalize statement: %s", sqlite3_errmsg(db_handle_));
        return false;
    }
    return true;
}

bool db_sqlite::exec(const char* cmd) {
    sqlite3_stmt* stmt = nullptr;
    if (!prep_stmt(cmd, &stmt)) {
        return false;
    }

    int ret = sqlite3_step(stmt);

    switch (ret) {
        case SQLITE_DONE:
            break;

        case SQLITE_ROW:
            LOG_DBG("getting values is not supported yet");
            sqlite3_reset(stmt);
            break;

        default:
            LOG_DBG("failed to evaluate the statement '%s': %s", cmd,
                    sqlite3_errmsg(db_handle_));
            return false;
    }

    return fin_stmt(stmt);
}

bool db_sqlite::exec_and_rst_stmt(sqlite3_stmt* stmt) {
    int ret = sqlite3_step(stmt);
    switch (ret) {
        case SQLITE_DONE:
            break;

        case SQLITE_ROW:
            LOG_DBG("getting values is not supported yet");
            sqlite3_reset(stmt);
            break;

        default:
            LOG_DBG("failed to evaluate the statement: %s",
                    sqlite3_errmsg(db_handle_));
            return false;
    }

    ret = sqlite3_clear_bindings(stmt);
    if (ret != SQLITE_OK) {
        LOG_DBG("failed to clear bindings: %s", sqlite3_errmsg(db_handle_));
        return false;
    }

    ret = sqlite3_reset(stmt);
    if (ret != SQLITE_OK) {
        LOG_DBG("failed to reset statement: %s", sqlite3_errmsg(db_handle_));
        return false;
    }

    if (within_transaction_) {
        ++transaction_size_;
        if (transaction_size_ > max_transaction_size_) {
            sync();
        }
    }
    return true;
}

std::string db_sqlite::get_sqlite_info() {
    std::string info = "sqlite library info\n";

    info += "  verison: " + std::string(sqlite3_libversion()) + "\n";
    info += "  threadsafe: " + std::to_string(sqlite3_threadsafe()) + "\n";
    return info;
}

bool db_sqlite::store_entry(entry_bp_hit* entry) {
    sqlite3_bind_int64(stmt_bp_hit_, 1, entry->timestamp_);
    sqlite3_bind_int64(stmt_bp_hit_, 2, entry->timestamp_rt_);
    sqlite3_bind_text(stmt_bp_hit_, 3, entry->target_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt_bp_hit_, 4, entry->bp_desc_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_bp_hit_);
    return true;
}

bool db_sqlite::store_entry(entry_bp_hit_desc* entry) {
    sqlite3_bind_text(stmt_bp_hit_desc_, 1, entry->desc_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_bp_hit_desc_);
    return true;
}

bool db_sqlite::store_entry(entry_cout* entry) {
    sqlite3_bind_int64(stmt_cout_, 1, entry->timestamp_);
    sqlite3_bind_int64(stmt_cout_, 2, entry->timestamp_rt_);
    sqlite3_bind_text(stmt_cout_, 3, entry->text_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_cout_);
    return true;
}

bool db_sqlite::store_entry(entry_exception* entry) {
    sqlite3_bind_int64(stmt_exception_, 1, entry->exc_class_);
    sqlite3_bind_int64(stmt_exception_, 2, entry->timestamp_entry_);
    sqlite3_bind_int64(stmt_exception_, 3, entry->timestamp_entry_rt_);
    sqlite3_bind_int64(stmt_exception_, 4, entry->timestamp_exit_);
    sqlite3_bind_int64(stmt_exception_, 5, entry->timestamp_exit_rt_);
    sqlite3_bind_text(stmt_exception_, 6, entry->target_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_exception_);
    return true;
}

bool db_sqlite::store_entry(entry_interrupt* entry) {
    sqlite3_bind_int64(stmt_interrupt_, 1, entry->number_);
    sqlite3_bind_int64(stmt_interrupt_, 2, entry->timestamp_entry_);
    sqlite3_bind_int64(stmt_interrupt_, 3, entry->timestamp_entry_rt_);
    sqlite3_bind_int64(stmt_interrupt_, 4, entry->timestamp_exit_);
    sqlite3_bind_int64(stmt_interrupt_, 5, entry->timestamp_exit_rt_);
    sqlite3_bind_text(stmt_interrupt_, 6, entry->target_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_interrupt_);
    return true;
}

bool db_sqlite::store_entry(entry_sniffer* entry) {
    sqlite3_bind_text(stmt_sniffer_, 1, entry->name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_sniffer_);
    return true;
}

bool db_sqlite::store_entry(entry_task_switch* entry) {
    sqlite3_bind_int64(stmt_task_switch_, 1, entry->pid_prev_);
    sqlite3_bind_text(stmt_task_switch_, 2, entry->name_prev_.c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt_task_switch_, 3, entry->pid_next_);
    sqlite3_bind_text(stmt_task_switch_, 4, entry->name_next_.c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt_task_switch_, 5, entry->timestamp_);
    sqlite3_bind_int64(stmt_task_switch_, 6, entry->timestamp_rt_);
    sqlite3_bind_text(stmt_task_switch_, 7, entry->target_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_task_switch_);
    return true;
}

bool db_sqlite::store_entry(entry_target* entry) {
    sqlite3_bind_text(stmt_target_, 1, entry->name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_target_);
    return true;
}

bool db_sqlite::store_entry(entry_transaction* entry) {
    sqlite3_bind_int64(stmt_transaction_, 1, entry->timestamp_fw_);
    sqlite3_bind_int64(stmt_transaction_, 2, entry->timestamp_fw_rt_);
    sqlite3_bind_int64(stmt_transaction_, 3, entry->timestamp_bw_);
    sqlite3_bind_int64(stmt_transaction_, 4, entry->timestamp_bw_rt_);
    sqlite3_bind_int64(stmt_transaction_, 5, entry->address_);
    sqlite3_bind_int64(stmt_transaction_, 6, entry->length_);
    sqlite3_bind_int(stmt_transaction_, 7, entry->write_ ? 1 : 0);
    sqlite3_bind_text(stmt_transaction_, 8, entry->sniffer_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt_transaction_, 9, entry->dmi_req_ ? 1 : 0);
    exec_and_rst_stmt(stmt_transaction_);
    return true;
}

bool db_sqlite::store_entry(entry_quantum* entry) {
    sqlite3_bind_int64(stmt_quantum_, 1, entry->length_);
    sqlite3_bind_int64(stmt_quantum_, 2, entry->timestamp_);
    sqlite3_bind_int64(stmt_quantum_, 3, entry->timestamp_rt_);
    sqlite3_bind_text(stmt_quantum_, 4, entry->target_name_.c_str(), -1,
                      SQLITE_TRANSIENT);

    exec_and_rst_stmt(stmt_quantum_);
    return true;
}

bool db_sqlite::store_entry(entry_wait_event* entry) {
    sqlite3_bind_int64(stmt_wait_event_, 1, entry->timestamp_begin_);
    sqlite3_bind_int64(stmt_wait_event_, 2, entry->timestamp_begin_rt_);
    sqlite3_bind_int64(stmt_wait_event_, 3, entry->timestamp_end_);
    sqlite3_bind_int64(stmt_wait_event_, 4, entry->timestamp_end_rt_);
    sqlite3_bind_text(stmt_wait_event_, 5, entry->target_name_.c_str(), -1,
                      SQLITE_TRANSIENT);

    exec_and_rst_stmt(stmt_wait_event_);
    return true;
}

bool db_sqlite::store_entry(entry_call_stack* entry) {
    // call stack
    sqlite3_bind_int64(stmt_call_stack_, 1, entry->timestamp_);
    sqlite3_bind_int64(stmt_call_stack_, 2, entry->timestamp_rt_);
    sqlite3_bind_text(stmt_call_stack_, 3, entry->target_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_call_stack_);

    int64_t call_stack_id = sqlite3_last_insert_rowid(db_handle_);

    // call stack entries
    unsigned int i = 0;
    for (auto cs_entry : entry->call_stack_) {
        if (cs_entry->store_symbol) {
            sqlite3_bind_text(stmt_symbol_, 1, cs_entry->symbol_name.c_str(),
                              -1, SQLITE_TRANSIENT);
            sqlite3_bind_int64(stmt_symbol_, 2, cs_entry->symbol_addr);
            sqlite3_bind_int64(stmt_symbol_, 3, cs_entry->symbol_size);
            exec_and_rst_stmt(stmt_symbol_);
        }

        // call stack entry
        sqlite3_bind_int64(stmt_call_stack_frame_, 1, call_stack_id);
        sqlite3_bind_int64(stmt_call_stack_frame_, 2, i++);
        sqlite3_bind_int64(stmt_call_stack_frame_, 3, cs_entry->pc);
        sqlite3_bind_int64(stmt_call_stack_frame_, 4, cs_entry->fp);
        sqlite3_bind_text(
            stmt_call_stack_frame_, 5,
            cs_entry->store_symbol ? cs_entry->symbol_name.c_str() : "", -1,
            SQLITE_TRANSIENT);
        exec_and_rst_stmt(stmt_call_stack_frame_);
    }

    return true;
}

bool db_sqlite::store_entry(entry_event* entry) {
    // module
    sqlite3_bind_text(stmt_module_, 1, entry->module_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_module_);

    // event
    sqlite3_bind_int64(stmt_event_, 1, entry->timestamp_);
    sqlite3_bind_int64(stmt_event_, 2, entry->timestamp_rt_);
    sqlite3_bind_int64(stmt_event_, 3, entry->delay_);
    sqlite3_bind_text(stmt_event_, 4, entry->event_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt_event_, 5, entry->module_name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_event_);
    return true;
}

bool db_sqlite::store_entry(entry_vcml_payload_irq* entry) {
    int vcml_payload_id =
        store_vcml_payload_entry<entry_vcml_payload_irq>(entry);

    sqlite3_bind_int(stmt_vcml_payload_irq_, 1, vcml_payload_id);
    sqlite3_bind_int(stmt_vcml_payload_irq_, 2, entry->active_ ? 1 : 0);

    exec_and_rst_stmt(stmt_vcml_payload_irq_);
    return true;
}

bool db_sqlite::store_entry(entry_vcml_payload_pci* entry) {
    int vcml_payload_id =
        store_vcml_payload_entry<entry_vcml_payload_pci>(entry);

    sqlite3_bind_int(stmt_vcml_payload_pci_, 1, vcml_payload_id);
    sqlite3_bind_int(stmt_vcml_payload_pci_, 2, entry->cmd_);
    sqlite3_bind_int(stmt_vcml_payload_pci_, 3, entry->resp_);
    sqlite3_bind_int(stmt_vcml_payload_pci_, 4, entry->addr_space_);
    sqlite3_bind_int64(stmt_vcml_payload_pci_, 5, entry->addr_);
    sqlite3_bind_int(stmt_vcml_payload_pci_, 6, entry->data_);
    sqlite3_bind_int(stmt_vcml_payload_pci_, 7, entry->size_);
    sqlite3_bind_int(stmt_vcml_payload_pci_, 8, entry->debug_ ? 1 : 0);

    exec_and_rst_stmt(stmt_vcml_payload_pci_);
    return true;
}

bool db_sqlite::store_entry(entry_vcml_payload_sd_cmd* entry) {
    int vcml_payload_id =
        store_vcml_payload_entry<entry_vcml_payload_sd_cmd>(entry);

    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 1, vcml_payload_id);
    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 2, entry->opcode_);
    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 3, entry->arg_);
    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 4, entry->crc_);
    sqlite3_bind_blob(stmt_vcml_payload_sd_cmd_, 5, entry->resp_,
                      entry->resp_len_, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 6, entry->resp_len_);
    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 7, entry->appcmd_ ? 1 : 0);
    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 8, entry->spi_ ? 1 : 0);
    sqlite3_bind_int(stmt_vcml_payload_sd_cmd_, 9, entry->status_);

    exec_and_rst_stmt(stmt_vcml_payload_sd_cmd_);
    return true;
}

bool db_sqlite::store_entry(entry_vcml_payload_sd_data* entry) {
    int vcml_payload_id =
        store_vcml_payload_entry<entry_vcml_payload_sd_data>(entry);

    sqlite3_bind_int(stmt_vcml_payload_sd_data_, 1, vcml_payload_id);
    sqlite3_bind_int(stmt_vcml_payload_sd_data_, 2, entry->mode_);
    sqlite3_bind_int(stmt_vcml_payload_sd_data_, 3, entry->data_);
    sqlite3_bind_int(stmt_vcml_payload_sd_data_, 4, entry->status_);

    exec_and_rst_stmt(stmt_vcml_payload_sd_data_);
    return true;
}

bool db_sqlite::store_entry(entry_vcml_payload_spi* entry) {
    int vcml_payload_id =
        store_vcml_payload_entry<entry_vcml_payload_spi>(entry);

    sqlite3_bind_int(stmt_vcml_payload_spi_, 1, vcml_payload_id);
    sqlite3_bind_int(stmt_vcml_payload_spi_, 2, entry->mosi_);
    sqlite3_bind_int(stmt_vcml_payload_spi_, 3, entry->miso_);

    exec_and_rst_stmt(stmt_vcml_payload_spi_);
    return true;
}

bool db_sqlite::store_entry(entry_vcml_payload_tlm* entry) {
    int vcml_payload_id =
        store_vcml_payload_entry<entry_vcml_payload_tlm>(entry);

    sqlite3_bind_int(stmt_vcml_payload_tlm_, 1, vcml_payload_id);
    sqlite3_bind_int64(stmt_vcml_payload_tlm_, 2, entry->address_);
    sqlite3_bind_int64(stmt_vcml_payload_tlm_, 3, entry->length_);
    sqlite3_bind_int(stmt_vcml_payload_tlm_, 4, entry->write_ ? 1 : 0);
    sqlite3_bind_int(stmt_vcml_payload_tlm_, 5, entry->dmi_ ? 1 : 0);

    exec_and_rst_stmt(stmt_vcml_payload_tlm_);
    return true;
}

bool db_sqlite::store_entry(entry_vcml_payload_virtio* entry) {
    int vcml_payload_id =
        store_vcml_payload_entry<entry_vcml_payload_virtio>(entry);

    sqlite3_bind_int(stmt_vcml_payload_virtio_, 1, vcml_payload_id);

    exec_and_rst_stmt(stmt_vcml_payload_virtio_);
    return true;
}

bool db_sqlite::store_entry(entry_sc_proc* entry) {
    sqlite3_bind_text(stmt_sc_proc_, 1, entry->name_.c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt_sc_proc_, 2, static_cast<int>(entry->type_));

    exec_and_rst_stmt(stmt_sc_proc_);
    return true;
}

bool db_sqlite::store_entry(entry_sc_sched* entry) {
    sqlite3_bind_int64(stmt_sc_sched_, 1, entry->timestamp_begin_st_);
    sqlite3_bind_int64(stmt_sc_sched_, 2, entry->timestamp_begin_rt_);
    sqlite3_bind_int64(stmt_sc_sched_, 3, entry->timestamp_end_st_);
    sqlite3_bind_int64(stmt_sc_sched_, 4, entry->timestamp_end_rt_);
    sqlite3_bind_text(stmt_sc_sched_, 5, entry->name_.c_str(), -1,
                      SQLITE_TRANSIENT);

    exec_and_rst_stmt(stmt_sc_sched_);
    return true;
}

void db_sqlite::start_transaction() {
    if (within_transaction_ || !use_transactions_) {
        return;
    }

    if (exec_and_rst_stmt(stmt_beg_trans_)) {
        within_transaction_ = true;
    } else {
        LOG_DBG("failed to begin transaction: %s", sqlite3_errmsg(db_handle_));
    }
}

void db_sqlite::end_transaction() {
    if (!within_transaction_) {
        return;
    }

    within_transaction_ = false;
    if (exec_and_rst_stmt(stmt_end_trans_)) {
        transaction_size_ = 0;
    } else {
        LOG_DBG("failed to end transaction: %s", sqlite3_errmsg(db_handle_));
    }
}

void db_sqlite::sync() {
    end_transaction();
    start_transaction();
}

std::string db_sqlite::get_type() const { return "db_sqlite"; }

void db_sqlite::set_max_transaction_size(
    const unsigned int max_transaction_size) {
    max_transaction_size_ = max_transaction_size > 0 ? max_transaction_size : 1;
    if (transaction_size_ >= max_transaction_size_) {
        sync();
    }
}

unsigned int db_sqlite::get_max_transaction_size() const {
    return max_transaction_size_;
}

}  // namespace simdal
