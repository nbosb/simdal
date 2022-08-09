/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_sc_sched.h"

#include <utility>

namespace simdal {

entry_sc_sched::entry_sc_sched() : entry_sc_sched(nullptr) {}

entry_sc_sched::entry_sc_sched(entry<entry_sc_sched>::mem_pool_t* mp)
    : entry<entry_sc_sched>(mp),
      name_(""),
      timestamp_begin_st_(0),
      timestamp_end_st_(0),
      timestamp_begin_rt_(0),
      timestamp_end_rt_(0) {}

void entry_sc_sched::configure(std::string name, uint64_t timestamp_begin_st,
                               uint64_t timestamp_end_st,
                               uint64_t timestamp_begin_rt,
                               uint64_t timestamp_end_rt) {
    name_ = std::move(name);
    timestamp_begin_st_ = timestamp_begin_st;
    timestamp_end_st_ = timestamp_end_st;
    timestamp_begin_rt_ = timestamp_begin_rt;
    timestamp_end_rt_ = timestamp_end_rt;
}

}  // namespace simdal
