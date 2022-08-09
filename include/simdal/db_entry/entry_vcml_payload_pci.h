/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_PCI_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_PCI_H_

#include <string>

#include "simdal/db_entry/entry_vcml_payload.h"

namespace simdal {

class entry_vcml_payload_pci
    : public entry_vcml_payload<entry_vcml_payload_pci> {
    friend entry<entry_vcml_payload_pci>::mem_pool_t;

  public:
    entry_vcml_payload_pci(const entry_vcml_payload_pci&) = delete;
    ~entry_vcml_payload_pci() override = default;

    int cmd_;
    int resp_;
    int addr_space_;
    uint64_t addr_;
    uint32_t data_;
    uint32_t size_;
    bool debug_;

  private:
    entry_vcml_payload_pci();
    explicit entry_vcml_payload_pci(
        entry<entry_vcml_payload_pci>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_PCI_H_
