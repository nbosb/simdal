/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_SNIFFER_H_
#define SIMDAL_DB_ENTRY_ENTRY_SNIFFER_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_sniffer : public entry<entry_sniffer> {
    friend entry<entry_sniffer>::mem_pool_t;

  public:
    entry_sniffer(const entry_sniffer&) = delete;
    ~entry_sniffer() override = default;

    void configure(std::string name);

    std::string name_;

  private:
    entry_sniffer();
    explicit entry_sniffer(entry<entry_sniffer>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_SNIFFER_H_
