/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_BP_HIT_H_
#define SIMDAL_DB_ENTRY_ENTRY_BP_HIT_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_bp_hit : public entry<entry_bp_hit> {
    friend entry<entry_bp_hit>::mem_pool_t;

  public:
    entry_bp_hit(const entry_bp_hit&) = delete;
    ~entry_bp_hit() override = default;

    void configure(std::string bp_desc, uint64_t timestamp,
                   uint64_t timestamp_rt, std::string target_name);

    std::string bp_desc_;
    uint64_t timestamp_;
    uint64_t timestamp_rt_;
    std::string target_name_;

  private:
    entry_bp_hit();
    explicit entry_bp_hit(entry<entry_bp_hit>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_BP_HIT_H_
