/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_EVENT_H_
#define SIMDAL_DB_ENTRY_ENTRY_EVENT_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_event : public entry<entry_event> {
    friend entry<entry_event>::mem_pool_t;

  public:
    entry_event(const entry_event&) = delete;
    ~entry_event() override = default;

    void configure(uint64_t timestamp, uint64_t timestamp_rt, uint64_t delay,
                   std::string module_name, std::string event_name);

    uint64_t timestamp_;
    uint64_t timestamp_rt_;
    uint64_t delay_;
    std::string module_name_;
    std::string event_name_;

  private:
    entry_event();
    explicit entry_event(entry<entry_event>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_EVENT_H_
