/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_IRQ_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_IRQ_H_

#include <string>

#include "simdal/db_entry/entry_vcml_payload.h"

namespace simdal {

class entry_vcml_payload_irq
    : public entry_vcml_payload<entry_vcml_payload_irq> {
    friend entry<entry_vcml_payload_irq>::mem_pool_t;

  public:
    entry_vcml_payload_irq(const entry_vcml_payload_irq&) = delete;
    ~entry_vcml_payload_irq() override = default;

    bool active_;

  private:
    entry_vcml_payload_irq();
    explicit entry_vcml_payload_irq(
        entry<entry_vcml_payload_irq>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_IRQ_H_
