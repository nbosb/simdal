/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_vcml_payload_sd_cmd.h"

#include <utility>

namespace simdal {

entry_vcml_payload_sd_cmd::entry_vcml_payload_sd_cmd()
    : entry_vcml_payload_sd_cmd(nullptr) {}

entry_vcml_payload_sd_cmd::entry_vcml_payload_sd_cmd(
    entry<entry_vcml_payload_sd_cmd>::mem_pool_t* mp)
    : entry_vcml_payload<entry_vcml_payload_sd_cmd>(mp,
                                                    payload_type_enum::SD_CMD),
      opcode_(0),
      arg_(0),
      crc_(0),
      resp_len_(0),
      appcmd_(false),
      spi_(false),
      status_(0) {}

}  // namespace simdal
