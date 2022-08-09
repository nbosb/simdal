/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_DB_ENTRY_ENTRY_H_
#define SIMDAL_DB_ENTRY_ENTRY_H_

#include <string>

#include "simdal/mem_pool.h"
#include "simdal/utils.h"

namespace simdal {

class entry_base {
  public:
    entry_base() = default;
    entry_base(const entry_base&) = delete;
    virtual ~entry_base() = default;

    virtual void free() = 0;
};

template <typename subclass>
class entry : public entry_base {
  public:
    typedef mem_pool<subclass> mem_pool_t;

    static subclass* create();
    entry() = delete;
    entry(entry&) = delete;
    ~entry() override = default;

    void free() override;

  protected:
    explicit entry(mem_pool_t* mp = nullptr);

  private:
    mem_pool_t* mp_;
    static mem_pool_t s_mp;
};

template <typename subclass>
typename entry<subclass>::mem_pool_t entry<subclass>::s_mp;

template <typename subclass>
entry<subclass>::entry(entry<subclass>::mem_pool_t* mp)
    : entry_base(), mp_(mp) {}

template <typename subclass>
subclass* entry<subclass>::create() {
    return s_mp.allocate();
}

template <typename subclass>
void entry<subclass>::free() {
    if (mp_ == nullptr) {
        LOG_DBG("called free() without valid mem_pool");
        return;
    }

    auto* sub = dynamic_cast<subclass*>(this);
    mp_->free(sub);
}

}  // namespace simdal
#endif  // SIMDAL_DB_ENTRY_ENTRY_H_
