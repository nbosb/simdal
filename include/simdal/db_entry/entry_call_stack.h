/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_CALL_STACK_H_
#define SIMDAL_DB_ENTRY_ENTRY_CALL_STACK_H_

#include <list>
#include <memory>
#include <string>

#include "simdal/db_entry/entry.h"

namespace simdal {

class entry_call_stack : public entry<entry_call_stack> {
    friend entry<entry_call_stack>::mem_pool_t;

  public:
    typedef struct function {
        uint64_t fp;
        uint64_t pc;
        std::string symbol_name;
        uint64_t symbol_addr;
        uint64_t symbol_size;
        bool store_symbol;

        function()
            : fp(0),
              pc(0),
              symbol_addr(0),
              symbol_size(0),
              store_symbol(false){};
        function(uint64_t fp, uint64_t pc)
            : fp(fp),
              pc(pc),
              symbol_addr(0),
              symbol_size(0),
              store_symbol(false){};
    } function_t;
    typedef std::shared_ptr<function_t> function_ptr_t;

    entry_call_stack(const entry_call_stack&) = delete;
    ~entry_call_stack() override = default;

    void configure(std::string target_name, uint64_t timestamp,
                   uint64_t timestamp_rt);

    std::string target_name_;
    uint64_t timestamp_;
    uint64_t timestamp_rt_;
    std::list<function_ptr_t> call_stack_;

  private:
    entry_call_stack();
    explicit entry_call_stack(entry<entry_call_stack>::mem_pool_t* mp);
};
}  // namespace simdal

#endif  // SIMDAL_DB_ENTRY_ENTRY_CALL_STACK_H_
