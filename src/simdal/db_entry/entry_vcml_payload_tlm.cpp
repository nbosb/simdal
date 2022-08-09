/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_vcml_payload_tlm.h"

#include <utility>

namespace simdal {

entry_vcml_payload_tlm::entry_vcml_payload_tlm()
    : entry_vcml_payload_tlm(nullptr) {}

entry_vcml_payload_tlm::entry_vcml_payload_tlm(
    entry<entry_vcml_payload_tlm>::mem_pool_t* mp)
    : entry_vcml_payload<entry_vcml_payload_tlm>(mp, payload_type_enum::TLM),
      write_(false),
      dmi_(false) {}

}  // namespace simdal
