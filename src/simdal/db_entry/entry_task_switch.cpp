/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_task_switch.h"

#include <utility>

namespace simdal {

entry_task_switch::entry_task_switch() : entry_task_switch(nullptr) {}

entry_task_switch::entry_task_switch(entry<entry_task_switch>::mem_pool_t* mp)
    : entry<entry_task_switch>(mp),
      pid_prev_(0),
      name_prev_(""),
      pid_next_(0),
      name_next_(""),
      timestamp_(0),
      timestamp_rt_(0),
      target_name_("") {}

void entry_task_switch::configure(uint64_t pid_prev, std::string name_prev,
                                  uint64_t pid_next, std::string name_next,
                                  uint64_t timestamp, uint64_t timestamp_rt,
                                  std::string target_name) {
    pid_prev_ = pid_prev;
    name_prev_ = std::move(name_prev);
    pid_next_ = pid_next;
    name_next_ = std::move(name_next);
    timestamp_ = timestamp;
    timestamp_rt_ = timestamp_rt;
    target_name_ = std::move(target_name);
}

}  // namespace simdal
