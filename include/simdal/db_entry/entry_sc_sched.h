
/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_SC_SCHED_H_
#define SIMDAL_DB_ENTRY_ENTRY_SC_SCHED_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_sc_sched : public entry<entry_sc_sched> {
    friend entry<entry_sc_sched>::mem_pool_t;

  public:
    entry_sc_sched(const entry_sc_sched&) = delete;
    ~entry_sc_sched() override = default;

    void configure(std::string name, uint64_t timestamp_begin_st,
                   uint64_t timestamp_end_st, uint64_t timestamp_begin_rt,
                   uint64_t timestamp_end_rt);

    std::string name_;
    uint64_t timestamp_begin_st_;
    uint64_t timestamp_end_st_;
    uint64_t timestamp_begin_rt_;
    uint64_t timestamp_end_rt_;

  private:
    entry_sc_sched();
    explicit entry_sc_sched(entry<entry_sc_sched>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_SC_SCHED_H_
