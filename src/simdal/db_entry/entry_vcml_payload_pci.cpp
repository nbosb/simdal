/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_vcml_payload_pci.h"

#include <utility>

namespace simdal {

entry_vcml_payload_pci::entry_vcml_payload_pci()
    : entry_vcml_payload_pci(nullptr) {}

entry_vcml_payload_pci::entry_vcml_payload_pci(
    entry<entry_vcml_payload_pci>::mem_pool_t* mp)
    : entry_vcml_payload<entry_vcml_payload_pci>(mp, payload_type_enum::PCI),
      cmd_(0),
      resp_(0),
      addr_space_(0),
      addr_(0),
      data_(0),
      size_(0),
      debug_(false) {}

}  // namespace simdal
