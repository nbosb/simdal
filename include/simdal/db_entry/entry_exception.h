/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_EXCEPTION_H_
#define SIMDAL_DB_ENTRY_ENTRY_EXCEPTION_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_exception : public entry<entry_exception> {
    friend entry<entry_exception>::mem_pool_t;

  public:
    entry_exception(const entry_exception&) = delete;
    ~entry_exception() override = default;

    void configure(uint64_t exc_class, uint64_t timestamp_entry,
                   uint64_t timestamp_entry_rt, uint64_t timestamp_exit,
                   uint64_t timestamp_exit_rt, std::string target_name);

    uint64_t exc_class_;
    uint64_t timestamp_entry_;
    uint64_t timestamp_exit_;
    uint64_t timestamp_entry_rt_;
    uint64_t timestamp_exit_rt_;
    std::string target_name_;

  private:
    entry_exception();
    explicit entry_exception(entry<entry_exception>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_EXCEPTION_H_
