/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_QUANTUM_H_
#define SIMDAL_DB_ENTRY_ENTRY_QUANTUM_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_quantum : public entry<entry_quantum> {
    friend entry<entry_quantum>::mem_pool_t;

  public:
    entry_quantum(const entry_quantum&) = delete;
    ~entry_quantum() override = default;

    void configure(uint64_t length, uint64_t timestamp, uint64_t timestamp_rt,
                   std::string target_name);

    uint64_t length_;
    uint64_t timestamp_;
    uint64_t timestamp_rt_;
    std::string target_name_;

  private:
    entry_quantum();
    explicit entry_quantum(entry<entry_quantum>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_QUANTUM_H_
