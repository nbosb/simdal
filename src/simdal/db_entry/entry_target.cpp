/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_target.h"

#include <utility>

namespace simdal {

entry_target::entry_target() : entry_target(nullptr) {}

entry_target::entry_target(entry<entry_target>::mem_pool_t* mp)
    : entry<entry_target>(mp), name_("") {}

void entry_target::configure(std::string name) { name_ = std::move(name); }

}  // namespace simdal
