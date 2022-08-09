/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_VIRTIO_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_VIRTIO_H_

#include <string>

#include "simdal/db_entry/entry_vcml_payload.h"

namespace simdal {

class entry_vcml_payload_virtio
    : public entry_vcml_payload<entry_vcml_payload_virtio> {
    friend entry<entry_vcml_payload_virtio>::mem_pool_t;

  public:
    entry_vcml_payload_virtio(const entry_vcml_payload_virtio&) = delete;
    ~entry_vcml_payload_virtio() override = default;

  private:
    entry_vcml_payload_virtio();
    explicit entry_vcml_payload_virtio(
        entry<entry_vcml_payload_virtio>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_VIRTIO_H_
