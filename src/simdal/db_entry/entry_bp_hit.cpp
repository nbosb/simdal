/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_bp_hit.h"

#include <utility>

namespace simdal {

entry_bp_hit::entry_bp_hit() : entry_bp_hit(nullptr) {}

entry_bp_hit::entry_bp_hit(entry<entry_bp_hit>::mem_pool_t* mp)
    : entry<entry_bp_hit>(mp),
      bp_desc_(""),
      timestamp_(0),
      timestamp_rt_(0),
      target_name_("") {}

void entry_bp_hit::configure(std::string bp_desc, uint64_t timestamp,
                             uint64_t timestamp_rt, std::string target_name) {
    bp_desc_ = bp_desc;
    timestamp_ = timestamp;
    timestamp_rt_ = timestamp_rt;
    target_name_ = std::move(target_name);
}

}  // namespace simdal
