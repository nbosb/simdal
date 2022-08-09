/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_TASK_SWITCH_H_
#define SIMDAL_DB_ENTRY_ENTRY_TASK_SWITCH_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_task_switch : public entry<entry_task_switch> {
    friend entry<entry_task_switch>::mem_pool_t;

  public:
    entry_task_switch(const entry_task_switch&) = delete;
    ~entry_task_switch() override = default;

    void configure(uint64_t pid_prev, std::string name_prev, uint64_t pid_next,
                   std::string name_next, uint64_t timestamp,
                   uint64_t timestamp_rt, std::string target_name);

    uint64_t pid_prev_;
    std::string name_prev_;
    uint64_t pid_next_;
    std::string name_next_;
    uint64_t timestamp_;
    uint64_t timestamp_rt_;
    std::string target_name_;

  private:
    entry_task_switch();
    explicit entry_task_switch(entry<entry_task_switch>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_TASK_SWITCH_H_
