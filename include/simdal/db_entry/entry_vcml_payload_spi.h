/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SPI_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SPI_H_

#include <string>

#include "simdal/db_entry/entry_vcml_payload.h"

namespace simdal {

class entry_vcml_payload_spi
    : public entry_vcml_payload<entry_vcml_payload_spi> {
    friend entry<entry_vcml_payload_spi>::mem_pool_t;

  public:
    entry_vcml_payload_spi(const entry_vcml_payload_spi&) = delete;
    ~entry_vcml_payload_spi() override = default;

    uint8_t mosi_;
    uint8_t miso_;

  private:
    entry_vcml_payload_spi();
    explicit entry_vcml_payload_spi(
        entry<entry_vcml_payload_spi>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SPI_H_
