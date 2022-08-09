/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_COUT_H_
#define SIMDAL_DB_ENTRY_ENTRY_COUT_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_cout : public entry<entry_cout> {
    friend entry<entry_cout>::mem_pool_t;

  public:
    entry_cout(const entry_cout&) = delete;
    ~entry_cout() override = default;

    void configure(uint64_t timestamp, uint64_t timestamp_rt, std::string text);

    uint64_t timestamp_;
    uint64_t timestamp_rt_;
    std::string text_;

  private:
    entry_cout();
    explicit entry_cout(entry<entry_cout>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_COUT_H_
