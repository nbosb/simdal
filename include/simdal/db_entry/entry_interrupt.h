/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_INTERRUPT_H_
#define SIMDAL_DB_ENTRY_ENTRY_INTERRUPT_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_interrupt : public entry<entry_interrupt> {
    friend entry<entry_interrupt>::mem_pool_t;

  public:
    entry_interrupt(const entry_interrupt&) = delete;
    ~entry_interrupt() override = default;

    void configure(uint64_t number, uint64_t timestamp_entry,
                   uint64_t timestamp_entry_rt, uint64_t timestamp_exit,
                   uint64_t timestamp_exit_rt, std::string target_name);

    uint64_t number_;
    uint64_t timestamp_entry_;
    uint64_t timestamp_exit_;
    uint64_t timestamp_entry_rt_;
    uint64_t timestamp_exit_rt_;
    std::string target_name_;

  private:
    entry_interrupt();
    explicit entry_interrupt(entry<entry_interrupt>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_INTERRUPT_H_
