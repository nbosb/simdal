/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_WAIT_EVENT_H_
#define SIMDAL_DB_ENTRY_ENTRY_WAIT_EVENT_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_wait_event : public entry<entry_wait_event> {
    friend entry<entry_wait_event>::mem_pool_t;

  public:
    entry_wait_event(const entry_wait_event&) = delete;
    ~entry_wait_event() override = default;

    void configure(uint64_t timestamp_begin, uint64_t timestamp_begin_rt,
                   uint64_t timestamp_end, uint64_t timestamp_end_rt,
                   std::string target_name);

    uint64_t timestamp_begin_;
    uint64_t timestamp_begin_rt_;
    uint64_t timestamp_end_;
    uint64_t timestamp_end_rt_;
    std::string target_name_;

  private:
    entry_wait_event();
    explicit entry_wait_event(entry<entry_wait_event>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_WAIT_EVENT_H_
