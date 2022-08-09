/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db/db_csv.h"

#include <algorithm>
#include <iomanip>

namespace simdal {

db_csv::db_csv(const std::string& path)
    : db(path),
      csv_bp_hit_(append_before_dot(path, "_bp")),
      csv_bp_hit_desc_(append_before_dot(path, "_bp_desc")),
      csv_call_stack_(append_before_dot(path, "_call_stack")),
      csv_call_stack_frame_(append_before_dot(path, "_call_stack_frame")),
      csv_cout_(append_before_dot(path, "_cout")),
      csv_event_(append_before_dot(path, "_event")),
      csv_exception_(append_before_dot(path, "_exc")),
      csv_interrupt_(append_before_dot(path, "_int")),
      csv_module_(append_before_dot(path, "_module")),
      csv_task_switch_(append_before_dot(path, "_ts")),
      csv_target_(append_before_dot(path, "_target")),
      csv_quantum_(append_before_dot(path, "_quantum")),
      csv_sc_proc_(append_before_dot(path, "_sc_proc")),
      csv_sc_sched_(append_before_dot(path, "_sc_sched")),
      csv_sniffer_(append_before_dot(path, "_snif")),
      csv_symbol_(append_before_dot(path, "_symbol")),
      csv_trans_(append_before_dot(path, "_trans")),
      csv_vcml_payload_(append_before_dot(path, "_vcml_payload")),
      csv_vcml_payload_irq_(append_before_dot(path, "_vcml_payload_irq")),
      csv_vcml_payload_pci_(append_before_dot(path, "_vcml_payload_pci")),
      csv_vcml_payload_sd_cmd_(append_before_dot(path, "_vcml_payload_sd_cmd")),
      csv_vcml_payload_sd_data_(
          append_before_dot(path, "_vcml_payload_sd_data")),
      csv_vcml_payload_spi_(append_before_dot(path, "_vcml_payload_spi")),
      csv_vcml_payload_tlm_(append_before_dot(path, "_vcml_payload_tlm")),
      csv_vcml_payload_virtio_(append_before_dot(path, "_vcml_payload_virtio")),
      csv_wait_event_(append_before_dot(path, "_wait_event")) {}

db_csv::~db_csv() {
    stop_db_worker_thread();
    if (is_open()) {
        db_csv::close();
    }
}

db_csv::ptr db_csv::create(const std::string& path) {
    return std::make_shared<db_csv>(path);
}

std::string db_csv::get_type() const { return "db_csv"; }

std::string db_csv::get_path() const {
    std::stringstream ss;
    ss << csv_bp_hit_.get_path() << ':' << csv_bp_hit_desc_.get_path() << ':'
       << csv_call_stack_.get_path() << ':' << csv_call_stack_frame_.get_path()
       << ':' << csv_cout_.get_path() << ':' << csv_event_.get_path() << ':'
       << csv_exception_.get_path() << ':' << csv_interrupt_.get_path() << ':'
       << ':' << csv_module_.get_path() << csv_task_switch_.get_path() << ':'
       << csv_target_.get_path() << ':' << csv_quantum_.get_path() << ':'
       << csv_sc_proc_.get_path() << ':' << csv_sc_sched_.get_path() << ':'
       << csv_sniffer_.get_path() << ':' << csv_symbol_.get_path() << ':'
       << csv_trans_.get_path() << ':' << csv_vcml_payload_.get_path() << ':'
       << csv_vcml_payload_irq_.get_path() << ':'
       << csv_vcml_payload_pci_.get_path() << ':'
       << csv_vcml_payload_sd_cmd_.get_path() << ':'
       << csv_vcml_payload_sd_data_.get_path() << ':'
       << csv_vcml_payload_spi_.get_path() << ':'
       << csv_vcml_payload_tlm_.get_path() << ':'
       << csv_vcml_payload_virtio_.get_path() << ':'
       << csv_wait_event_.get_path() << ':' << csv_module_.get_path() << ':'
       << csv_task_switch_.get_path();
    return ss.str();
}

bool db_csv::open() {
    bool success = true;
    success = success && csv_bp_hit_.open();
    success = success && csv_bp_hit_desc_.open();
    success = success && csv_call_stack_.open();
    success = success && csv_call_stack_frame_.open();
    success = success && csv_cout_.open();
    success = success && csv_event_.open();
    success = success && csv_exception_.open();
    success = success && csv_interrupt_.open();
    success = success && csv_module_.open();
    success = success && csv_task_switch_.open();
    success = success && csv_target_.open();
    success = success && csv_quantum_.open();
    success = success && csv_sc_proc_.open();
    success = success && csv_sc_sched_.open();
    success = success && csv_sniffer_.open();
    success = success && csv_symbol_.open();
    success = success && csv_trans_.open();
    success = success && csv_vcml_payload_.open();
    success = success && csv_vcml_payload_irq_.open();
    success = success && csv_vcml_payload_pci_.open();
    success = success && csv_vcml_payload_sd_cmd_.open();
    success = success && csv_vcml_payload_sd_data_.open();
    success = success && csv_vcml_payload_spi_.open();
    success = success && csv_vcml_payload_tlm_.open();
    success = success && csv_vcml_payload_virtio_.open();
    success = success && csv_wait_event_.open();
    return success;
}

bool db_csv::close() {
    bool success = true;
    success = csv_bp_hit_.close() && success;
    success = csv_bp_hit_desc_.close() && success;
    success = csv_call_stack_.close() && success;
    success = csv_call_stack_frame_.close() && success;
    success = csv_cout_.close() && success;
    success = csv_exception_.close() && success;
    success = csv_interrupt_.close() && success;
    success = csv_task_switch_.close() && success;
    success = csv_target_.close() && success;
    success = csv_quantum_.close() && success;
    success = csv_sc_proc_.close() && success;
    success = csv_sc_sched_.close() && success;
    success = csv_sniffer_.close() && success;
    success = csv_symbol_.close() && success;
    success = csv_trans_.close() && success;
    success = csv_vcml_payload_.close() && success;
    success = csv_vcml_payload_irq_.close() && success;
    success = csv_vcml_payload_pci_.close() && success;
    success = csv_vcml_payload_sd_cmd_.close() && success;
    success = csv_vcml_payload_sd_data_.close() && success;
    success = csv_vcml_payload_spi_.close() && success;
    success = csv_vcml_payload_tlm_.close() && success;
    success = csv_vcml_payload_virtio_.close() && success;
    success = csv_wait_event_.close() && success;
    return success;
}

bool db_csv::init() {
    bool success = true;

    std::list<std::string> headlines_bp_hit = {
        "id", "timestamp", "timestamp_rt", "target_id", "bp_id"};
    std::list<std::string> headlines_bp_hit_desc = {"id", "bp_desc"};
    std::list<std::string> headlines_exceptions = {
        "exc_class",      "timestamp_entry",   "timestamp_entry_rt",
        "timestamp_exit", "timestamp_exit_rt", "target_id"};
    std::list<std::string> headlines_interrupts = {
        "interrupt_nr",   "timestamp_entry",   "timestamp_entry_rt",
        "timestamp_exit", "timestamp_exit_rt", "target_id"};
    std::list<std::string> headlines_task_switch = {
        "pid_prev",  "name_prev",    "pid_next", "name_next",
        "timestamp", "timestamp_rt", "target_id"};
    std::list<std::string> headlines_targets = {"id", "name"};
    std::list<std::string> headlines_quantum = {
        "length", "timestamp", "timestamp", "timestamp_rt", "target_id"};
    std::list<std::string> headlines_sniffer = {"id", "name"};
    std::list<std::string> headlines_trans = {
        "timestamp_fw",    "timestamp_fw_rt", "timestamp_bw",
        "timestamp_bw_rt", "address",         "length",
        "write",           "sniffer_id",      "dmi_req"};
    std::list<std::string> headlines_cout = {"timestamp", "timestamp_rt",
                                             "text"};
    std::list<std::string> headlines_wait_event = {
        "timestamp_begin", "timestamp_begin_rt", "timestamp_end",
        "timestamp_end_rt", "target_id"};
    std::list<std::string> headlines_call_stack = {"id", "timestamp",
                                                   "timestamp_rt", "target_id"};
    std::list<std::string> headlines_call_stack_frame = {
        "call_stack_id", "level", "pc", "fp", "symbol_id"};
    std::list<std::string> headlines_call_symbol = {"id", "name", "address",
                                                    "size"};
    std::list<std::string> headlines_event = {"timestamp", "timestamp_rt",
                                              "delay", "event", "module_id"};
    std::list<std::string> headlines_module = {"id", "name"};
    std::list<std::string> headlines_vcml_payload = {
        "id",           "timestamp_fw",    "timestamp_fw_rt",
        "timestamp_bw", "timestamp_bw_rt", "port",
        "module_id"};
    std::list<std::string> headlines_vcml_payload_irq = {"vcml_payload_id",
                                                         "active"};
    std::list<std::string> headlines_vcml_payload_pci = {
        "vcml_payload_id", "command", "response", "address_spcae",
        "address",         "data",    "size",     "debug"};
    std::list<std::string> headlines_vcml_payload_sd_cmd = {
        "vcml_payload_id", "opcode", "arg", "crc",   "resp",
        "resp_len",        "appcmd", "spi", "status"};
    std::list<std::string> headlines_vcml_payload_sd_data = {
        "vcml_payload_id", "mode", "data", "status"};
    std::list<std::string> headlines_vcml_payload_spi = {"vcml_payload_id",
                                                         "mosi", "miso"};
    std::list<std::string> headlines_vcml_payload_tlm = {
        "vcml_payload_id", "address", "length", "write", "dmi"};
    std::list<std::string> headlines_sc_proc = {"id", "name", "type"};
    std::list<std::string> headlines_sc_sched = {
        "timestamp_begin", "timestamp_begin_rt", "timestamp_end",
        "timestamp_end_rt", "sc_proc_id"};
    std::list<std::string> headlines_vcml_payload_virtio = {"vcml_payload_id"};

    success = success && csv_bp_hit_.write(headlines_bp_hit);
    success = success && csv_bp_hit_desc_.write(headlines_bp_hit_desc);
    success = success && csv_call_stack_.write(headlines_call_stack);
    success =
        success && csv_call_stack_frame_.write(headlines_call_stack_frame);
    success = success && csv_cout_.write(headlines_cout);
    success = success && csv_event_.write(headlines_event);
    success = success && csv_exception_.write(headlines_exceptions);
    success = success && csv_interrupt_.write(headlines_interrupts);
    success = success && csv_module_.write(headlines_module);
    success = success && csv_task_switch_.write(headlines_task_switch);
    success = success && csv_target_.write(headlines_targets);
    success = success && csv_quantum_.write(headlines_quantum);
    success = success && csv_sc_proc_.write(headlines_sc_proc);
    success = success && csv_sc_sched_.write(headlines_sc_sched);
    success = success && csv_sniffer_.write(headlines_sniffer);
    success = success && csv_symbol_.write(headlines_call_symbol);
    success = success && csv_trans_.write(headlines_trans);
    success = success && csv_vcml_payload_.write(headlines_vcml_payload);
    success =
        success && csv_vcml_payload_irq_.write(headlines_vcml_payload_irq);
    success =
        success && csv_vcml_payload_pci_.write(headlines_vcml_payload_pci);
    success = success &&
              csv_vcml_payload_sd_cmd_.write(headlines_vcml_payload_sd_cmd);
    success = success &&
              csv_vcml_payload_sd_data_.write(headlines_vcml_payload_sd_data);
    success =
        success && csv_vcml_payload_spi_.write(headlines_vcml_payload_spi);
    success =
        success && csv_vcml_payload_tlm_.write(headlines_vcml_payload_tlm);
    success = success &&
              csv_vcml_payload_virtio_.write(headlines_vcml_payload_virtio);
    success = success && csv_wait_event_.write(headlines_wait_event);

    return success;
}

bool db_csv::find_target_id(const std::string& name, unsigned int* id) {
    bool found = false;
    std::string line;
    return csv_target_.get_id_of(name, 1, 0, found, line, *id, 1);
}

bool db_csv::find_sniffer_id(const std::string& name, unsigned int* id) {
    bool found = false;
    std::string line;
    return csv_sniffer_.get_id_of(name, 1, 0, found, line, *id, 1);
}

bool db_csv::find_bp_desc_id(const std::string& bp_desc, unsigned int* id) {
    bool found = false;
    std::string line;
    return csv_bp_hit_desc_.get_id_of(bp_desc, 1, 0, found, line, *id, 1);
}

bool db_csv::find_sc_proc_id(const std::string& sc_proc, unsigned int* id) {
    bool found = false;
    std::string line;
    return csv_sc_proc_.get_id_of(sc_proc, 1, 0, found, line, *id, 1);
}

/*
 * <pid_prev>;<name_prev>;<pid_next>;<name_next>;<timestamp>;<timestamp_rt>;<target_id>
 */
bool db_csv::store_entry(entry_task_switch* entry) {
    unsigned int target_id = 0;

    if (find_target_id(entry->target_name_, &target_id)) {
        std::stringstream ss;
        ss << entry->pid_prev_ << ';' << entry->name_prev_ << ';'
           << entry->pid_next_ << ';' << entry->name_next_ << ';'
           << entry->timestamp_ << ';' << entry->timestamp_rt_ << ';'
           << target_id;
        return csv_task_switch_.write(ss.str());
    }

    return false;
}

/*
 * <id>;<name>
 */
bool db_csv::store_entry(entry_target* entry) {
    std::string line;

    csv_target_.reset_read_pos();
    csv_target_.read_line(line);  // skip header

    unsigned int id = 0;
    bool found = false;
    if (!csv_target_.get_id_of(entry->name_, 1, 0, found, line, id, 1)) {
        return false;
    }

    if (!found) {
        std::stringstream ss;
        ss << id << ';' << entry->name_;
        return csv_target_.write(ss.str());
    }

    return false;
}

/*
 * <number>;<timestamp_entry>;<timestamp_entry_rt>;<timestamp_exit>;<timestamp_exit_rt>;<target_id>
 */
bool db_csv::store_entry(entry_interrupt* entry) {
    unsigned int target_id = 0;

    if (find_target_id(entry->target_name_, &target_id)) {
        std::stringstream ss;
        ss << entry->number_ << ';' << entry->timestamp_entry_ << ';'
           << entry->timestamp_entry_rt_ << ';' << entry->timestamp_exit_ << ';'
           << entry->timestamp_exit_rt_ << ';' << target_id;
        return csv_interrupt_.write(ss.str());
    }

    return false;
}

/*
 * <exc_class>;<timestamp_entry>;<timestamp_entry_rt>;<timestamp_exit>;<timestamp_exit_rt>;<target_id>
 */
bool db_csv::store_entry(entry_exception* entry) {
    unsigned int target_id = 0;

    if (find_target_id(entry->target_name_, &target_id)) {
        std::stringstream ss;
        ss << entry->exc_class_ << ';' << entry->timestamp_entry_ << ';'
           << entry->timestamp_entry_rt_ << ';' << entry->timestamp_exit_ << ';'
           << entry->timestamp_exit_rt_ << ';' << target_id;
        return csv_exception_.write(ss.str());
    }

    return false;
}

/*
 * <bp_id>;<timestamp>;<timestamp_rt>;<target_id>
 */
bool db_csv::store_entry(entry_bp_hit* entry) {
    unsigned int target_id = 0;
    unsigned int bp_desc_id = 0;

    if (!find_target_id(entry->target_name_, &target_id)) {
        return false;
    }

    if (!find_bp_desc_id(entry->bp_desc_, &bp_desc_id)) {
        return false;
    }

    std::stringstream ss;
    ss << bp_desc_id << ';' << entry->timestamp_ << ';' << entry->timestamp_rt_
       << ';' << target_id;
    return csv_bp_hit_.write(ss.str());
}

/*
 * <timestamp>;<timestamp_rt>;<text>
 */
bool db_csv::store_entry(entry_cout* entry) {
    std::stringstream ss;
    ss << entry->timestamp_ << ';' << entry->timestamp_rt_ << ';'
       << entry->text_;
    return csv_cout_.write(ss.str());
}

/*
 * <id>;<desc>
 */
bool db_csv::store_entry(entry_bp_hit_desc* entry) {
    std::string line;

    csv_bp_hit_desc_.reset_read_pos();
    csv_bp_hit_desc_.read_line(line);  // skip header

    unsigned int id = 0;
    bool found = false;
    if (!csv_bp_hit_desc_.get_id_of(entry->desc_, 1, 0, found, line, id, 1)) {
        return false;
    }

    if (!found) {
        std::stringstream ss;
        ss << id << ';' << entry->desc_;
        return csv_bp_hit_desc_.write(ss.str());
    }

    return false;
}

/*
 * <id>;<name>
 */
bool db_csv::store_entry(entry_sniffer* entry) {
    std::string line;

    csv_sniffer_.reset_read_pos();
    csv_sniffer_.read_line(line);  // skip header

    unsigned int id = 0;
    bool found = false;
    if (!csv_sniffer_.get_id_of(entry->name_, 1, 0, found, line, id, 1)) {
        return false;
    }

    if (!found) {
        std::stringstream ss;
        ss << id << ';' << entry->name_;
        return csv_sniffer_.write(ss.str());
    }

    return false;
}

/*
 * <timestamp_fw>;<timestamp_fw_rt>;<timestamp_bw>;<timestamp_bw_rt>;<address>;<length>;<write>;<dmi_req>;<sniffer_id>
 */
bool db_csv::store_entry(entry_transaction* entry) {
    unsigned int sniffer_id = 0;

    if (find_sniffer_id(entry->sniffer_name_, &sniffer_id)) {
        std::stringstream ss;
        ss << entry->timestamp_fw_ << ';' << entry->timestamp_fw_rt_ << ';'
           << entry->timestamp_bw_ << ';' << entry->timestamp_bw_rt_ << ';'
           << entry->address_ << ';' << entry->length_ << ';' << entry->write_
           << ';' << entry->dmi_req_ << ';' << sniffer_id;
        return csv_trans_.write(ss.str());
    }

    return false;
}

/*
 * <length>;<timestamp>;<timestamp_rt>;<target_id>
 */
bool db_csv::store_entry(entry_quantum* entry) {
    unsigned int target_id = 0;

    if (!find_target_id(entry->target_name_, &target_id)) {
        return false;
    }

    std::stringstream ss;
    ss << entry->length_ << ';' << entry->timestamp_ << ';'
       << entry->timestamp_rt_ << ';' << target_id;
    return csv_quantum_.write(ss.str());
}

/*
 * <timestamp_entry>;<timestamp_entry_rt>;<timestamp_end>;<timestamp_end_rt>;<target_id>
 */
bool db_csv::store_entry(entry_wait_event* entry) {
    unsigned int target_id = 0;

    if (!find_target_id(entry->target_name_, &target_id)) {
        return false;
    }

    std::stringstream ss;
    ss << entry->timestamp_begin_ << ';' << entry->timestamp_begin_rt_ << ';'
       << entry->timestamp_end_ << ';' << entry->timestamp_end_rt_ << ';'
       << target_id;
    return csv_wait_event_.write(ss.str());
}

bool db_csv::store_entry(entry_call_stack* entry) {
    unsigned int target_id = 0;
    bool success = true;

    if (!find_target_id(entry->target_name_, &target_id)) {
        return false;
    }

    unsigned int call_stack_id = csv_call_stack_.get_line_cnt();

    // call_stack: <id>;<timestamp>;<timestamp_rt>;<target_id>
    std::stringstream ss;
    ss << call_stack_id << ';' << entry->timestamp_ << ';'
       << entry->timestamp_rt_ << ';' << target_id;
    success = csv_call_stack_.write(ss.str()) && success;
    ss.str("");
    ss.clear();

    // call stack frames
    unsigned int i = 0;
    for (auto cs_frame : entry->call_stack_) {
        unsigned int symbol_id = 0;
        if (cs_frame->store_symbol) {
            std::string line;

            success = csv_symbol_.reset_read_pos() &&
                      csv_symbol_.read_line(line) &&
                      success;  // reset position and skip header

            bool found = false;
            if (csv_symbol_.get_id_of(cs_frame->symbol_name, 1, 0, found, line,
                                      symbol_id, 1)) {
                if (!found) {
                    // symbol: <id>;<name>;<address>;<size>
                    ss << symbol_id << ';' << cs_frame->symbol_name << ';'
                       << cs_frame->symbol_addr << ';' << cs_frame->symbol_size;
                    success = csv_symbol_.write(ss.str()) && success;
                    ss.str("");
                    ss.clear();
                }
            }
        }

        // call stack frame: <call_stack_id>;<level>;<pc>;<fp>;<symbol_id>
        ss << call_stack_id << ';' << i++ << ';' << cs_frame->pc << ';'
           << cs_frame->fp << ';' << symbol_id;
        success = csv_call_stack_frame_.write(ss.str()) && success;
        ss.str("");
        ss.clear();
    }
    return success;
}

bool db_csv::store_entry(entry_event* entry) {
    bool success = true;
    unsigned int module_id = get_module_id(entry->module_name_);
    std::stringstream ss;

    // event: <timestamp>;<timestamp_rt>;<delay>;<event_name>;<module_id>
    ss << entry->timestamp_ << ';' << entry->timestamp_rt_ << ';'
       << entry->delay_ << ';' << entry->event_name_ << ';' << module_id;

    success = success && csv_event_.write(ss.str());
    return success;
}

bool db_csv::store_entry(entry_vcml_payload_irq* entry) {
    unsigned int id = store_vcml_payload_entry<entry_vcml_payload_irq>(entry);
    std::stringstream ss;

    //<vcml_payload_id>;<active>
    ss << id << ';' << entry->active_;

    return csv_vcml_payload_irq_.write(ss.str());
}

bool db_csv::store_entry(entry_vcml_payload_pci* entry) {
    unsigned int id = store_vcml_payload_entry<entry_vcml_payload_pci>(entry);
    std::stringstream ss;

    //<vcml_payload_id>;<command>;<response>;<address_space>;<address>;<data>;<size>;<debug>
    ss << id << ';' << entry->cmd_ << ';' << entry->resp_ << ';'
       << entry->addr_space_ << ';' << entry->data_ << ';' << entry->size_
       << ';' << entry->debug_;

    return csv_vcml_payload_pci_.write(ss.str());
}

bool db_csv::store_entry(entry_vcml_payload_sd_cmd* entry) {
    unsigned int id =
        store_vcml_payload_entry<entry_vcml_payload_sd_cmd>(entry);
    std::stringstream ss;

    //<vcml_payload_id>;<opcode>;<arg>;<crc>;<resp>;<rep_len>;<appcmd>;<spi>;<status>
    ss << id << ';' << static_cast<unsigned int>(entry->opcode_) << ';'
       << entry->arg_ << ';' << static_cast<unsigned int>(entry->crc_) << ';';

    const char orig_fill = ss.fill();
    int orig_width = ss.width();

    // resp
    ss << "0x" << std::setfill('0') << std::setw(2) << std::hex;
    for (unsigned int i = 0; i < entry->resp_len_; ++i) {
        ss << static_cast<unsigned int>(entry->resp_[i]);
    }
    ss << std::setfill(orig_fill) << std::setw(orig_width) << std::dec;

    ss << ';' << entry->resp_len_ << ';' << entry->appcmd_ << ';' << entry->spi_
       << ';' << entry->status_;

    return csv_vcml_payload_sd_cmd_.write(ss.str());
}

bool db_csv::store_entry(entry_vcml_payload_sd_data* entry) {
    unsigned int id =
        store_vcml_payload_entry<entry_vcml_payload_sd_data>(entry);
    std::stringstream ss;

    //<vcml_payload_id>;<mode>;<data>;<status>
    ss << id << ';' << entry->mode_ << ';'
       << static_cast<unsigned int>(entry->data_) << ';' << entry->status_;

    return csv_vcml_payload_sd_data_.write(ss.str());
}

bool db_csv::store_entry(entry_vcml_payload_spi* entry) {
    unsigned int id = store_vcml_payload_entry<entry_vcml_payload_spi>(entry);
    std::stringstream ss;

    //<vcml_payload_id>;<mosi>;<miso>
    ss << id << ';' << static_cast<unsigned int>(entry->mosi_) << ';'
       << static_cast<unsigned int>(entry->miso_);

    return csv_vcml_payload_spi_.write(ss.str());
}

bool db_csv::store_entry(entry_vcml_payload_tlm* entry) {
    unsigned int id = store_vcml_payload_entry<entry_vcml_payload_tlm>(entry);
    std::stringstream ss;

    //<vcml_payload_id>;<address>;<length>;<write>;<dmi>
    ss << id << ';' << entry->address_ << ';' << entry->length_ << ';'
       << entry->write_ << ';' << entry->dmi_;

    return csv_vcml_payload_tlm_.write(ss.str());
}

bool db_csv::store_entry(entry_vcml_payload_virtio* entry) {
    unsigned int id =
        store_vcml_payload_entry<entry_vcml_payload_virtio>(entry);
    std::stringstream ss;

    //<vcml_payload_id>
    ss << id;

    return csv_vcml_payload_virtio_.write(ss.str());
}

bool db_csv::store_entry(entry_sc_proc* entry) {
    std::string line;

    csv_sc_proc_.reset_read_pos();
    csv_sc_proc_.read_line(line);  // skip header

    unsigned int id = 0;
    bool found = false;
    if (!csv_sc_proc_.get_id_of(entry->name_, 1, 0, found, line, id, 1)) {
        return false;
    }

    if (!found) {
        std::stringstream ss;
        ss << id << ';' << entry->name_ << ';'
           << static_cast<int>(entry->type_);
        return csv_sc_proc_.write(ss.str());
    }

    return false;
}

bool db_csv::store_entry(entry_sc_sched* entry) {
    unsigned int sc_proc_id;
    if (find_sc_proc_id(entry->name_, &sc_proc_id)) {
        std::stringstream ss;

        ss << entry->timestamp_begin_st_ << ';' << entry->timestamp_begin_rt_
           << ';' << entry->timestamp_end_st_ << ';' << entry->timestamp_end_rt_
           << ';' << sc_proc_id;
        return csv_sc_sched_.write(ss.str());
    }

    return false;
}

unsigned int db_csv::get_module_id(const std::string& name) {
    std::string line;
    bool found_module_name = false;
    unsigned int module_id = 0;

    csv_module_.reset_read_pos();  // reset position
    csv_module_.read_line(line);   // skip header
    if (!csv_module_.get_id_of(name, 1, 0, found_module_name, line, module_id,
                               1)) {
        return 0;
    }

    if (!found_module_name) {
        std::stringstream ss;
        // module: <id>;<name>
        ss << module_id << ';' << name;
        csv_module_.write(ss.str());
    }
    return module_id;
}

}  // namespace simdal
