/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_MEM_POOL_H_
#define SIMDAL_MEM_POOL_H_

#include <mutex>

namespace simdal {
template <typename T>
class mem_pool {
  public:
    mem_pool();
    mem_pool(mem_pool&) = delete;
    virtual ~mem_pool();

    T* allocate();
    void free(T*& elem);

  private:
    typedef std::lock_guard<std::mutex> lock_guard_t;
    typedef struct embedding {
        T* elem;
        struct embedding* next;
    } embedding_t;

    embedding_t* unused_embeddings_;
    embedding_t* unused_elem_;
    std::mutex mtx_;
};

template <typename T>
mem_pool<T>::mem_pool() : unused_embeddings_(nullptr), unused_elem_(nullptr) {}

template <typename T>
mem_pool<T>::~mem_pool() {
    // free all unused elements and their embeddings
    while (unused_elem_ != nullptr) {
        embedding_t* emb = unused_elem_;
        delete emb->elem;
        unused_elem_ = emb->next;
        delete emb;
    }

    // free all used embeddings
    while (unused_embeddings_ != nullptr) {
        embedding_t* emb = unused_embeddings_;
        unused_embeddings_ = emb->next;
        delete emb;
    }
}

template <typename T>
T* mem_pool<T>::allocate() {
    const lock_guard_t lock(mtx_);
    T* ret_val = nullptr;

    if (unused_elem_ == nullptr) {
        ret_val = new T(this);
    } else {
        embedding_t* emb = unused_elem_;
        unused_elem_ = emb->next;
        emb->next = unused_embeddings_;
        unused_embeddings_ = emb;

        ret_val = emb->elem;
    }
    return ret_val;
}

template <typename T>
void mem_pool<T>::free(T*& elem) {
    const lock_guard_t lock(mtx_);

    embedding_t* emb = unused_embeddings_;
    if (emb == nullptr) {
        emb = new embedding_t();
    } else {
        unused_embeddings_ = emb->next;
    }
    emb->next = unused_elem_;
    unused_elem_ = emb;
    emb->elem = elem;

    elem = nullptr;
}

}  // namespace simdal
#endif  // SIMDAL_MEM_POOL_H_
