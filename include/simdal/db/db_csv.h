/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_DB_CSV_H_
#define SIMDAL_DB_DB_CSV_H_

#include <memory>
#include <string>

#include "simdal/csv/csv_file.h"
#include "simdal/db/db.h"

namespace simdal {

class db_csv : public db {
  public:
    typedef std::shared_ptr<db_csv> ptr;

    explicit db_csv(const std::string& path);
    db_csv() = delete;
    db_csv(const db_csv& db_csv) = delete;
    ~db_csv() override;
    static ptr create(const std::string& path);

    std::string get_type() const override;
    std::string get_path() const override;

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
    bool find_target_id(const std::string& name, unsigned int* id);
    bool find_sniffer_id(const std::string& name, unsigned int* id);
    bool find_bp_desc_id(const std::string& bp_desc, unsigned int* id);
    bool find_sc_proc_id(const std::string& bp_desc, unsigned int* id);

    unsigned int get_module_id(const std::string& name);

    template <typename entry_type>
    unsigned int store_vcml_payload_entry(entry_type* entry);

    csv_file csv_bp_hit_;
    csv_file csv_bp_hit_desc_;
    csv_file csv_call_stack_;
    csv_file csv_call_stack_frame_;
    csv_file csv_cout_;
    csv_file csv_event_;
    csv_file csv_exception_;
    csv_file csv_interrupt_;
    csv_file csv_module_;
    csv_file csv_task_switch_;
    csv_file csv_target_;
    csv_file csv_quantum_;
    csv_file csv_sc_proc_;
    csv_file csv_sc_sched_;
    csv_file csv_sniffer_;
    csv_file csv_symbol_;
    csv_file csv_trans_;
    csv_file csv_vcml_payload_;
    csv_file csv_vcml_payload_irq_;
    csv_file csv_vcml_payload_pci_;
    csv_file csv_vcml_payload_sd_cmd_;
    csv_file csv_vcml_payload_sd_data_;
    csv_file csv_vcml_payload_spi_;
    csv_file csv_vcml_payload_tlm_;
    csv_file csv_vcml_payload_virtio_;
    csv_file csv_wait_event_;
};

template <typename entry_type>
unsigned int db_csv::store_vcml_payload_entry(entry_type* entry) {
    unsigned int id = csv_vcml_payload_.get_line_cnt();
    std::stringstream ss;

    // <id>;<timestamp_fw>;<timestamp_fw_rt>;<timestamp_bw>;<timestamp_bw_rt>;<port>;<module_id>
    ss << id << ';' << entry->timestamp_fw_ << ';' << entry->timestamp_fw_rt_
       << ';' << entry->timestamp_bw_ << ';' << entry->timestamp_bw_rt_ << ';'
       << entry->port_ << ';' << get_module_id(entry->module_);

    csv_vcml_payload_.write(ss.str());
    return id;
}

}  // namespace simdal
#endif  // SIMDAL_DB_DB_CSV_H_
