/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SD_CMD_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SD_CMD_H_

#include <string>

#include "simdal/db_entry/entry_vcml_payload.h"

namespace simdal {

class entry_vcml_payload_sd_cmd
    : public entry_vcml_payload<entry_vcml_payload_sd_cmd> {
    friend entry<entry_vcml_payload_sd_cmd>::mem_pool_t;

  public:
    entry_vcml_payload_sd_cmd(const entry_vcml_payload_sd_cmd&) = delete;
    ~entry_vcml_payload_sd_cmd() override = default;

    uint8_t opcode_;
    uint32_t arg_;
    uint8_t crc_;
    uint8_t resp_[17];
    uint32_t resp_len_;
    bool appcmd_;
    bool spi_;
    int status_;

  private:
    entry_vcml_payload_sd_cmd();
    explicit entry_vcml_payload_sd_cmd(
        entry<entry_vcml_payload_sd_cmd>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_SD_CMD_H_
