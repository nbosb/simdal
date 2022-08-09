/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_BP_HIT_DESC_H_
#define SIMDAL_DB_ENTRY_ENTRY_BP_HIT_DESC_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_bp_hit_desc : public entry<entry_bp_hit_desc> {
    friend entry<entry_bp_hit_desc>::mem_pool_t;

  public:
    entry_bp_hit_desc(const entry_bp_hit_desc&) = delete;
    ~entry_bp_hit_desc() override = default;

    void configure(std::string desc);

    std::string desc_;

  private:
    entry_bp_hit_desc();
    explicit entry_bp_hit_desc(entry<entry_bp_hit_desc>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_BP_HIT_DESC_H_
