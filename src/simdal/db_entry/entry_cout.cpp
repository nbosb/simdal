/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_cout.h"

#include <utility>

namespace simdal {

entry_cout::entry_cout() : entry_cout(nullptr) {}

entry_cout::entry_cout(entry<entry_cout>::mem_pool_t* mp)
    : entry<entry_cout>(mp), timestamp_(0), timestamp_rt_(0), text_("") {}

void entry_cout::configure(uint64_t timestamp, uint64_t timestamp_rt,
                           std::string text) {
    timestamp_ = timestamp;
    timestamp_rt_ = timestamp_rt;
    text_ = std::move(text);
}

}  // namespace simdal
