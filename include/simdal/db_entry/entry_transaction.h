/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_TRANSACTION_H_
#define SIMDAL_DB_ENTRY_ENTRY_TRANSACTION_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_transaction : public entry<entry_transaction> {
    friend entry<entry_transaction>::mem_pool_t;

  public:
    entry_transaction(const entry_transaction&) = delete;
    ~entry_transaction() override = default;

    void configure(uint64_t timestamp_fw, uint64_t timestamp_fw_rt,
                   uint64_t timestamp_bw, uint64_t timestamp_bw_rt,
                   uint64_t address, uint32_t length, bool write,
                   std::string sniffer_name, bool dmi_req);

    uint64_t timestamp_fw_;
    uint64_t timestamp_bw_;
    uint64_t timestamp_fw_rt_;
    uint64_t timestamp_bw_rt_;
    uint64_t address_;
    uint32_t length_;
    bool write_;
    std::string sniffer_name_;
    uint64_t sniffer_id_;
    bool dmi_req_;

  private:
    entry_transaction();
    explicit entry_transaction(entry<entry_transaction>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_TRANSACTION_H_
