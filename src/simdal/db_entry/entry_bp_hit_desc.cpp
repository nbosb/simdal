/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_bp_hit_desc.h"

#include <utility>

namespace simdal {

entry_bp_hit_desc::entry_bp_hit_desc() : entry_bp_hit_desc(nullptr) {}

entry_bp_hit_desc::entry_bp_hit_desc(entry<entry_bp_hit_desc>::mem_pool_t* mp)
    : entry<entry_bp_hit_desc>(mp), desc_("") {}

void entry_bp_hit_desc::configure(std::string desc) { desc_ = std::move(desc); }

}  // namespace simdal
