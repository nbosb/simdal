/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_exception.h"

#include <utility>

namespace simdal {

entry_exception::entry_exception() : entry_exception(nullptr) {}

entry_exception::entry_exception(entry<entry_exception>::mem_pool_t* mp)
    : entry<entry_exception>(mp),
      exc_class_(0),
      timestamp_entry_(0),
      timestamp_exit_(0),
      timestamp_entry_rt_(0),
      timestamp_exit_rt_(0),
      target_name_("") {}

void entry_exception::configure(uint64_t exc_class, uint64_t timestamp_entry,
                                uint64_t timestamp_entry_rt,
                                uint64_t timestamp_exit,
                                uint64_t timestamp_exit_rt,
                                std::string target_name) {
    exc_class_ = exc_class;
    timestamp_entry_ = timestamp_entry;
    timestamp_exit_ = timestamp_exit;
    timestamp_entry_rt_ = timestamp_entry_rt;
    timestamp_exit_rt_ = timestamp_exit_rt;
    target_name_ = std::move(target_name);
}

}  // namespace simdal
