/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_sniffer.h"

#include <utility>

namespace simdal {

entry_sniffer::entry_sniffer() : entry_sniffer(nullptr) {}

entry_sniffer::entry_sniffer(entry<entry_sniffer>::mem_pool_t* mp)
    : entry<entry_sniffer>(mp), name_("") {}

void entry_sniffer::configure(std::string name) { name_ = std::move(name); }

}  // namespace simdal
