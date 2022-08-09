/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_DB_SQLITE_H_
#define SIMDAL_DB_DB_SQLITE_H_

#include <memory>
#include <string>

#include "simdal/db/db.h"
#include "sqlite3.h"

namespace simdal {

class db_sqlite : public db {
  public:
    typedef std::shared_ptr<db_sqlite> ptr;

    explicit db_sqlite(const std::string& path);
    db_sqlite() = delete;
    db_sqlite(const db_sqlite& db_sqlite) = delete;
    ~db_sqlite() override;
    static ptr create(const std::string& path);

    std::string get_type() const override;
    void set_max_transaction_size(unsigned int max_transaction_size);
    unsigned int get_max_transaction_size() const;

    static std::string get_sqlite_info();

  protected:
    bool open() override;
    bool close() override;
    bool init() override;
    bool store_entry(entry_bp_hit* entry) override;
    bool store_entry(entry_bp_hit_desc* entry) override;
    bool store_entry(entry_call_stack* entry) override;
    bool store_entry(entry_cout* entry) override;
    bool store_entry(entry_event* entry) override;
    bool store_entry(entry_exception* entry) override;
    bool store_entry(entry_interrupt* entry) override;
    bool store_entry(entry_quantum* entry) override;
    bool store_entry(entry_sc_proc* entry) override;
    bool store_entry(entry_sc_sched* entry) override;
    bool store_entry(entry_sniffer* entry) override;
    bool store_entry(entry_task_switch* entry) override;
    bool store_entry(entry_target* entry) override;
    bool store_entry(entry_transaction* entry) override;
    bool store_entry(entry_vcml_payload_irq* entry) override;
    bool store_entry(entry_vcml_payload_pci* entry) override;
    bool store_entry(entry_vcml_payload_sd_cmd* entry) override;
    bool store_entry(entry_vcml_payload_sd_data* entry) override;
    bool store_entry(entry_vcml_payload_spi* entry) override;
    bool store_entry(entry_vcml_payload_tlm* entry) override;
    bool store_entry(entry_vcml_payload_virtio* entry) override;
    bool store_entry(entry_wait_event* entry) override;

  private:
    bool exec(const char* cmd);
    bool exec_and_rst_stmt(sqlite3_stmt* stmt);
    bool prep_stmt(const char* cmd, sqlite3_stmt** stmt);
    bool fin_stmt(sqlite3_stmt* stmt);
    bool prepare_statements();
    bool finalize_statements();
    void start_transaction();
    void end_transaction();
    void sync();

    template <typename entry_type>
    int store_vcml_payload_entry(entry_type* entry);

    sqlite3* db_handle_;
    unsigned int transaction_size_;
    unsigned int max_transaction_size_;
    bool within_transaction_;
    bool use_transactions_;

    sqlite3_stmt* stmt_beg_trans_;
    sqlite3_stmt* stmt_call_stack_;
    sqlite3_stmt* stmt_call_stack_frame_;
    sqlite3_stmt* stmt_end_trans_;
    sqlite3_stmt* stmt_bp_hit_;
    sqlite3_stmt* stmt_bp_hit_desc_;
    sqlite3_stmt* stmt_cout_;
    sqlite3_stmt* stmt_event_;
    sqlite3_stmt* stmt_exception_;
    sqlite3_stmt* stmt_interrupt_;
    sqlite3_stmt* stmt_module_;
    sqlite3_stmt* stmt_quantum_;
    sqlite3_stmt* stmt_sc_proc_;
    sqlite3_stmt* stmt_sc_sched_;
    sqlite3_stmt* stmt_sniffer_;
    sqlite3_stmt* stmt_symbol_;
    sqlite3_stmt* stmt_target_;
    sqlite3_stmt* stmt_task_switch_;
    sqlite3_stmt* stmt_transaction_;
    sqlite3_stmt* stmt_vcml_payload_;
    sqlite3_stmt* stmt_vcml_payload_irq_;
    sqlite3_stmt* stmt_vcml_payload_pci_;
    sqlite3_stmt* stmt_vcml_payload_sd_cmd_;
    sqlite3_stmt* stmt_vcml_payload_sd_data_;
    sqlite3_stmt* stmt_vcml_payload_spi_;
    sqlite3_stmt* stmt_vcml_payload_tlm_;
    sqlite3_stmt* stmt_vcml_payload_virtio_;
    sqlite3_stmt* stmt_wait_event_;

    static const std::string sc_cmd_beg_trans_;
    static const std::string sc_cmd_end_trans_;
    static const std::string sc_cmd_bp_hit_;
    static const std::string sc_cmd_bp_hit_desc_;
    static const std::string sc_cmd_call_stack_;
    static const std::string sc_cmd_call_stack_frame_;
    static const std::string sc_cmd_cout_;
    static const std::string sc_cmd_event_;
    static const std::string sc_cmd_exception_;
    static const std::string sc_cmd_interrupt_;
    static const std::string sc_cmd_module_;
    static const std::string sc_cmd_quantum_;
    static const std::string sc_cmd_sc_proc_;
    static const std::string sc_cmd_sc_sched_;
    static const std::string sc_cmd_sniffer_;
    static const std::string sc_cmd_symbol_;
    static const std::string sc_cmd_target_;
    static const std::string sc_cmd_task_switch_;
    static const std::string sc_cmd_transaction_;
    static const std::string sc_cmd_vcml_payload_;
    static const std::string sc_cmd_vcml_payload_irq_;
    static const std::string sc_cmd_vcml_payload_pci_;
    static const std::string sc_cmd_vcml_payload_sd_cmd_;
    static const std::string sc_cmd_vcml_payload_sd_data_;
    static const std::string sc_cmd_vcml_payload_spi_;
    static const std::string sc_cmd_vcml_payload_tlm_;
    static const std::string sc_cmd_vcml_payload_virtio_;
    static const std::string sc_cmd_wait_event_;
};

template <typename entry_type>
int db_sqlite::store_vcml_payload_entry(entry_type* entry) {
    // module
    sqlite3_bind_text(stmt_module_, 1, entry->module_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_module_);

    // vcml_payload
    sqlite3_bind_int64(stmt_vcml_payload_, 1, entry->timestamp_fw_);
    sqlite3_bind_int64(stmt_vcml_payload_, 2, entry->timestamp_fw_rt_);
    sqlite3_bind_int64(stmt_vcml_payload_, 3, entry->timestamp_bw_);
    sqlite3_bind_int64(stmt_vcml_payload_, 4, entry->timestamp_bw_rt_);
    sqlite3_bind_text(stmt_vcml_payload_, 5, entry->port_.c_str(), -1,
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt_vcml_payload_, 6, entry->module_.c_str(), -1,
                      SQLITE_TRANSIENT);
    exec_and_rst_stmt(stmt_vcml_payload_);
    return sqlite3_last_insert_rowid(db_handle_);
}

}  // namespace simdal
#endif  // SIMDAL_DB_DB_SQLITE_H_
