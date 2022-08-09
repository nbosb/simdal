/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_SC_PROC_H_
#define SIMDAL_DB_ENTRY_ENTRY_SC_PROC_H_

#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_sc_proc : public entry<entry_sc_proc> {
    friend entry<entry_sc_proc>::mem_pool_t;

  public:
    enum class process_type {
        NONE = 0,
        SC_THREAD = 1,
        SC_CTHREAD = 2,
        SC_METHOD = 3
    };

    entry_sc_proc(const entry_sc_proc&) = delete;
    ~entry_sc_proc() override = default;

    void configure(std::string name, process_type type);

    std::string name_;
    process_type type_;

  private:
    entry_sc_proc();
    explicit entry_sc_proc(entry<entry_sc_proc>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_SC_PROC_H_
