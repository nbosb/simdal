/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_H_
#define SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

template <typename subclass>
class entry_vcml_payload : public entry<subclass> {
    friend typename entry<subclass>::mem_pool_t;

  public:
    enum class payload_type_enum {
        TLM,
        IRQ,
        PCI,
        SPI,
        SD_CMD,
        SD_DATA,
        VIRTIO
    };

    entry_vcml_payload(const entry_vcml_payload&) = delete;
    ~entry_vcml_payload() override = default;

    uint64_t timestamp_fw_;
    uint64_t timestamp_fw_rt_;
    uint64_t timestamp_bw_;
    uint64_t timestamp_bw_rt_;
    payload_type_enum payload_type_;
    std::string port_;
    std::string module_;

  protected:
    entry_vcml_payload();
    explicit entry_vcml_payload(typename entry<subclass>::mem_pool_t* mp,
                                payload_type_enum payload_type);
};

template <typename subclass>
entry_vcml_payload<subclass>::entry_vcml_payload()
    : entry_vcml_payload(nullptr) {}

template <typename subclass>
entry_vcml_payload<subclass>::entry_vcml_payload(
    typename entry<subclass>::mem_pool_t* mp, payload_type_enum payload_type)
    : entry<subclass>(mp), payload_type_(payload_type) {}

}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_VCML_PAYLOAD_H_
