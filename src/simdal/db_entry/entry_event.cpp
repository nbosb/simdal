/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_event.h"

#include <utility>

namespace simdal {

entry_event::entry_event() : entry_event(nullptr) {}

entry_event::entry_event(entry<entry_event>::mem_pool_t* mp)
    : entry<entry_event>(mp), timestamp_(0), timestamp_rt_(0), delay_(0) {}

void entry_event::configure(uint64_t timestamp, uint64_t timestamp_rt,
                            uint64_t delay, std::string module_name,
                            std::string event_name) {
    timestamp_ = timestamp;
    timestamp_rt_ = timestamp_rt;
    delay_ = delay;
    module_name_ = std::move(module_name);
    event_name_ = std::move(event_name);
}

}  // namespace simdal
