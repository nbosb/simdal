/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db_entry/entry_vcml_payload_sd_data.h"

#include <utility>

namespace simdal {

entry_vcml_payload_sd_data::entry_vcml_payload_sd_data()
    : entry_vcml_payload_sd_data(nullptr) {}

entry_vcml_payload_sd_data::entry_vcml_payload_sd_data(
    entry<entry_vcml_payload_sd_data>::mem_pool_t* mp)
    : entry_vcml_payload<entry_vcml_payload_sd_data>(
          mp, payload_type_enum::SD_DATA),
      mode_(0),
      data_(0),
      status_(0) {}

}  // namespace simdal
