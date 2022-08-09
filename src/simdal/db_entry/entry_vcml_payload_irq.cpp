/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_vcml_payload_irq.h"

#include <utility>

namespace simdal {

entry_vcml_payload_irq::entry_vcml_payload_irq()
    : entry_vcml_payload_irq(nullptr) {}

entry_vcml_payload_irq::entry_vcml_payload_irq(
    entry<entry_vcml_payload_irq>::mem_pool_t* mp)
    : entry_vcml_payload<entry_vcml_payload_irq>(mp, payload_type_enum::IRQ),
      active_(false) {}

}  // namespace simdal
