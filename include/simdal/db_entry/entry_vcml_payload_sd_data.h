/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SD_DATA_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SD_DATA_H_

#include <string>

#include "simdal/db_entry/entry_vcml_payload.h"

namespace simdal {

class entry_vcml_payload_sd_data
    : public entry_vcml_payload<entry_vcml_payload_sd_data> {
    friend entry<entry_vcml_payload_sd_data>::mem_pool_t;

  public:
    entry_vcml_payload_sd_data(const entry_vcml_payload_sd_data&) = delete;
    ~entry_vcml_payload_sd_data() override = default;

    int mode_;
    uint8_t data_;
    int status_;

  private:
    entry_vcml_payload_sd_data();
    explicit entry_vcml_payload_sd_data(
        entry<entry_vcml_payload_sd_data>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SD_DATA_H_
