/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_wait_event.h"

#include <utility>

namespace simdal {

entry_wait_event::entry_wait_event() : entry_wait_event(nullptr) {}

entry_wait_event::entry_wait_event(entry<entry_wait_event>::mem_pool_t* mp)
    : entry<entry_wait_event>(mp),
      timestamp_begin_(0),
      timestamp_begin_rt_(0),
      timestamp_end_(0),
      timestamp_end_rt_(0),
      target_name_("") {}

void entry_wait_event::configure(uint64_t timestamp_begin,
                                 uint64_t timestamp_begin_rt,
                                 uint64_t timestamp_end,
                                 uint64_t timestamp_end_rt,
                                 std::string target_name) {
    timestamp_begin_ = timestamp_begin;
    timestamp_begin_rt_ = timestamp_begin_rt;
    timestamp_end_ = timestamp_end;
    timestamp_end_rt_ = timestamp_end_rt;
    target_name_ = std::move(target_name);
}

}  // namespace simdal
