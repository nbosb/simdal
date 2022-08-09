/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_call_stack.h"

#include <utility>

namespace simdal {

entry_call_stack::entry_call_stack() : entry_call_stack(nullptr) {}

entry_call_stack::entry_call_stack(entry<entry_call_stack>::mem_pool_t* mp)
    : entry<entry_call_stack>(mp), timestamp_(0), timestamp_rt_(0) {}

void entry_call_stack::configure(std::string target_name, uint64_t timestamp,
                                 uint64_t timestamp_rt) {
    target_name_ = std::move(target_name);
    timestamp_ = timestamp;
    timestamp_rt_ = timestamp_rt;
    call_stack_.clear();
}

}  // namespace simdal
