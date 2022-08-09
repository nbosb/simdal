/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_TLM_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_TLM_H_

#include <string>

#include "simdal/db_entry/entry_vcml_payload.h"

namespace simdal {

class entry_vcml_payload_tlm
    : public entry_vcml_payload<entry_vcml_payload_tlm> {
    friend entry<entry_vcml_payload_tlm>::mem_pool_t;

  public:
    entry_vcml_payload_tlm(const entry_vcml_payload_tlm&) = delete;
    ~entry_vcml_payload_tlm() override = default;

    uint64_t address_;
    uint64_t length_;
    bool write_;
    bool dmi_;

  private:
    entry_vcml_payload_tlm();
    explicit entry_vcml_payload_tlm(
        entry<entry_vcml_payload_tlm>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_TLM_H_
