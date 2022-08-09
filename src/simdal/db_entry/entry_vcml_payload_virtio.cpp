/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_vcml_payload_virtio.h"

#include <utility>

namespace simdal {

entry_vcml_payload_virtio::entry_vcml_payload_virtio()
    : entry_vcml_payload_virtio(nullptr) {}

entry_vcml_payload_virtio::entry_vcml_payload_virtio(
    entry<entry_vcml_payload_virtio>::mem_pool_t* mp)
    : entry_vcml_payload<entry_vcml_payload_virtio>(
          mp, payload_type_enum::VIRTIO) {}

}  // namespace simdal
