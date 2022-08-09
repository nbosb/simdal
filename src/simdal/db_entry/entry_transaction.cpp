/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_transaction.h"

#include <utility>

namespace simdal {

entry_transaction::entry_transaction() : entry_transaction(nullptr) {}

entry_transaction::entry_transaction(entry<entry_transaction>::mem_pool_t* mp)
    : entry<entry_transaction>(mp),
      timestamp_fw_(0),
      timestamp_bw_(0),
      timestamp_fw_rt_(0),
      timestamp_bw_rt_(0),
      address_(0),
      length_(0),
      write_(false),
      sniffer_name_(""),
      sniffer_id_(0),
      dmi_req_(false) {}

void entry_transaction::configure(uint64_t timestamp_fw,
                                  uint64_t timestamp_fw_rt,
                                  uint64_t timestamp_bw,
                                  uint64_t timestamp_bw_rt, uint64_t address,
                                  uint32_t length, bool write,
                                  std::string sniffer_name, bool dmi_req) {
    timestamp_fw_ = timestamp_fw;
    timestamp_bw_ = timestamp_bw;
    timestamp_fw_rt_ = timestamp_fw_rt;
    timestamp_bw_rt_ = timestamp_bw_rt;
    address_ = address;
    length_ = length;
    write_ = write;
    sniffer_name_ = std::move(sniffer_name);
    dmi_req_ = dmi_req;
}

}  // namespace simdal
