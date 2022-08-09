/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_quantum.h"

#include <utility>

namespace simdal {

entry_quantum::entry_quantum() : entry_quantum(nullptr) {}

entry_quantum::entry_quantum(entry<entry_quantum>::mem_pool_t* mp)
    : entry<entry_quantum>(mp),
      length_(0),
      timestamp_(0),
      timestamp_rt_(0),
      target_name_("") {}

void entry_quantum::configure(uint64_t length, uint64_t timestamp,
                              uint64_t timestamp_rt, std::string target_name) {
    length_ = length;
    timestamp_ = timestamp;
    timestamp_rt_ = timestamp_rt;
    target_name_ = std::move(target_name);
}

}  // namespace simdal
