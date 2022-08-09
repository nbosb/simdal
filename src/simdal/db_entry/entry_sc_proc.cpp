/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_sc_proc.h"

#include <utility>

namespace simdal {

entry_sc_proc::entry_sc_proc() : entry_sc_proc(nullptr) {}

entry_sc_proc::entry_sc_proc(entry<entry_sc_proc>::mem_pool_t* mp)
    : entry<entry_sc_proc>(mp), name_(""), type_(process_type::NONE) {}

void entry_sc_proc::configure(std::string name, process_type type) {
    name_ = std::move(name);
    type_ = type;
}

}  // namespace simdal
