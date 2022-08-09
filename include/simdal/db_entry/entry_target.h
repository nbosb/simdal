/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_TARGET_H_
#define SIMDAL_DB_ENTRY_ENTRY_TARGET_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_target : public entry<entry_target> {
    friend entry<entry_target>::mem_pool_t;

  public:
    entry_target(const entry_target&) = delete;
    ~entry_target() override = default;

    void configure(std::string name);

    std::string name_;

  private:
    entry_target();
    explicit entry_target(entry<entry_target>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_TARGET_H_
