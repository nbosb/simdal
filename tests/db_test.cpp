/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/db/db.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <cstdio>
#include <mutex>
#include <thread>

#include "simdal/db/db_sqlite.h"
#include "simdal/db_entry/entry.h"
#include "simdal/mem_pool.h"
#include "simdal/utils.h"

class mock_entry : public simdal::entry<mock_entry> {
  public:
    explicit mock_entry(simdal::mem_pool<mock_entry>* mp = nullptr)
        : simdal::entry<mock_entry>(mp), id_(0) {
        std::lock_guard<std::mutex> lock(constructor_calls_mtx_);
        constructor_calls_++;
    }

    virtual ~mock_entry() {
        std::lock_guard<std::mutex> lock(destructor_calls_mtx_);
        destructor_calls_++;
    }

    MOCK_METHOD0(get_sql_statement, const char*());
    MOCK_CONST_METHOD0(get_csv_line, std::string());

    static unsigned int get_constructor_calls() {
        std::lock_guard<std::mutex> lock(constructor_calls_mtx_);
        return constructor_calls_;
    }

    static unsigned int get_destructor_calls() {
        std::lock_guard<std::mutex> lock(destructor_calls_mtx_);
        return destructor_calls_;
    }

    static void reset_constructor_destructor_calls() {
        {
            std::lock_guard<std::mutex> lock(constructor_calls_mtx_);
            constructor_calls_ = 0;
        }
        {
            std::lock_guard<std::mutex> lock(destructor_calls_mtx_);
            destructor_calls_ = 0;
        }
    }

    unsigned int id_;

  private:
    static unsigned int constructor_calls_;
    static unsigned int destructor_calls_;
    static std::mutex constructor_calls_mtx_;
    static std::mutex destructor_calls_mtx_;
};

unsigned int mock_entry::constructor_calls_ = 0;
unsigned int mock_entry::destructor_calls_ = 0;
std::mutex mock_entry::constructor_calls_mtx_;
std::mutex mock_entry::destructor_calls_mtx_;

class mock_db : public simdal::db {
  public:
    mock_db() : simdal::db("/my/test/path"), id_sum_(0) {
        // store_entry is the critical function which is just allowed to be
        // called once at a time
        ON_CALL(*this, store_entry(testing::An<simdal::entry_base*>()))
            .WillByDefault([this](simdal::entry_base* entry) -> bool {
                bool lock_succeed = mtx_.try_lock();
                EXPECT_TRUE(lock_succeed);  // expect that the mutex is not
                                            // locked -> threadsafe criterion!

                // block for 1 ms
                std::this_thread::sleep_for(std::chrono::milliseconds{1});

                id_sum_ += dynamic_cast<mock_entry*>(entry)
                               ->id_;  // add ids -> used to check if all entrys
                                       // are correctly added to the db

                if (lock_succeed) {
                    mtx_.unlock();
                }
                return true;
            });
    }

    MOCK_METHOD0(open, bool());
    MOCK_METHOD0(close, bool());
    MOCK_METHOD0(init, bool());
    MOCK_METHOD1(store_entry, bool(simdal::entry_base* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_bp_hit* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_bp_hit_desc* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_call_stack* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_cout* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_event* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_exception* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_interrupt* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_quantum* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_sc_proc* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_sc_sched* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_sniffer* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_target* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_task_switch* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_transaction* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_vcml_payload_irq* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_vcml_payload_pci* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_vcml_payload_sd_cmd* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_vcml_payload_sd_data* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_vcml_payload_spi* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_vcml_payload_tlm* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_vcml_payload_virtio* entry));
    MOCK_METHOD1(store_entry, bool(simdal::entry_wait_event* entry));

    const unsigned int get_id_sum() const { return id_sum_; }

    mock_entry* create_entry_mock(unsigned int id = 0) {
        auto* e = mp.allocate();
        e->id_ = id;
        return e;
    }

  private:
    std::mutex mtx_;
    unsigned int id_sum_;

    simdal::mem_pool<mock_entry> mp;
};

simdal::entry_task_switch* create_entry_ts(simdal::db* db) {
    simdal::entry_task_switch* entry_ts = simdal::entry_task_switch::create();
    entry_ts->configure(0, "task0", 1, "task1", 2, 3, "cpu0");
    return entry_ts;
}

TEST(db_test, open_close) {
    mock_entry::reset_constructor_destructor_calls();
    {
        mock_db db;

        // all operations should fail because no db has been opened
        EXPECT_FALSE(db.store(db.create_entry_mock()));
        EXPECT_FALSE(db.store(create_entry_ts(&db)));

        // test behaviour when open fails
        EXPECT_CALL(db, open())
            .Times(1)
            .WillOnce(testing::Return(false));  // let open fail
        EXPECT_CALL(db, init())
            .Times(0);  // expect that init is not called if open fails
        EXPECT_FALSE(db.open_db());  // expect that open fails
        EXPECT_FALSE(db.is_open());

        // all operations should fail because db failed to open
        EXPECT_FALSE(db.store(db.create_entry_mock()));
        EXPECT_FALSE(db.store(create_entry_ts(&db)));
        EXPECT_FALSE(db.close_db());

        // test behaviour when open succeeds and init fails
        EXPECT_CALL(db, open())
            .Times(1)
            .WillOnce(testing::Return(true));  // let open succeed
        EXPECT_CALL(db, init())
            .Times(1)
            .WillOnce(testing::Return(false));  // expect that init is called if
                                                // open succeeds and fails
        EXPECT_CALL(db, close())
            .Times(1)
            .WillOnce(testing::Return(
                true));  // close should be called because init failed
        EXPECT_FALSE(db.open_db());  // expect that open fails
        EXPECT_FALSE(db.is_open());

        // all operations should fail because db failed to init
        EXPECT_FALSE(db.store(db.create_entry_mock()));
        EXPECT_FALSE(db.store(create_entry_ts(&db)));
        EXPECT_FALSE(db.close_db());

        // test opening and closing of the database multiple times
        for (unsigned int i = 0; i < 2; i++) {
            // test behaviour when open and init succeed
            EXPECT_CALL(db, open())
                .Times(1)
                .WillOnce(testing::Return(true));  // let open succeed
            EXPECT_CALL(db, init())
                .Times(1)
                .WillOnce(testing::Return(
                    true));  // expect that init is called if open succeeds
            EXPECT_TRUE(db.open_db());  // expect that open succeeds
            EXPECT_TRUE(db.is_open());

            EXPECT_FALSE(
                db.open_db());  // expect that another call of open_db fails
                                // because there is already an opened db
            EXPECT_TRUE(db.is_open());

            // all operations should succeed
            EXPECT_CALL(db, store_entry(testing::An<simdal::entry_base*>()))
                .Times(2)
                .WillRepeatedly(testing::Return(true));
            EXPECT_TRUE(db.store(db.create_entry_mock()));
            EXPECT_TRUE(db.store(create_entry_ts(&db)));

            EXPECT_CALL(db, close()).Times(1).WillOnce(testing::Return(true));
            db.close_db();
            EXPECT_FALSE(db.is_open());

            // all operations should fail because no db has been opened
            EXPECT_FALSE(db.store(db.create_entry_mock()));
            EXPECT_FALSE(db.store(create_entry_ts(&db)));
        }
    }

    // check if all created mock_entries got destroied
    EXPECT_EQ(mock_entry::get_constructor_calls(),
              mock_entry::get_destructor_calls());
}

void thread_func(mock_db* db, unsigned int start_id, unsigned int end_id,
                 unsigned int* id_sum) {
    for (unsigned int id = start_id; id <= end_id; id++) {
        db->store(db->create_entry_mock(id));
        *id_sum += id;
    }
}

TEST(db_test, threadsafe) {
    unsigned int expected_id_sum = 0;
    unsigned int id_sum = 0;

    mock_entry::reset_constructor_destructor_calls();
    {
        mock_db db;

        EXPECT_CALL(db, open())
            .Times(1)
            .WillOnce(testing::Return(true));  // let open succeed
        EXPECT_CALL(db, init())
            .Times(1)
            .WillOnce(testing::Return(
                true));  // expect that init is called if open succeeds
        EXPECT_TRUE(db.open_db());  // expect that open succeeds

        const unsigned int elements_per_thread = 100;
        unsigned int start_index_t1 = 0;
        unsigned int start_index_t2 = start_index_t1 + elements_per_thread;
        unsigned int start_index_t3 = start_index_t2 + elements_per_thread;
        unsigned int start_index_t4 = start_index_t3 + elements_per_thread;

        unsigned int id_sum_t1 = 0;
        unsigned int id_sum_t2 = 0;
        unsigned int id_sum_t3 = 0;
        unsigned int id_sum_t4 = 0;

        EXPECT_CALL(db, store_entry(testing::An<simdal::entry_base*>()))
            .Times(elements_per_thread * 4);
        std::thread worker1 =
            std::thread(thread_func, &db, start_index_t1,
                        start_index_t1 + elements_per_thread - 1, &id_sum_t1);
        std::thread worker2 =
            std::thread(thread_func, &db, start_index_t2,
                        start_index_t2 + elements_per_thread - 1, &id_sum_t2);
        std::thread worker3 =
            std::thread(thread_func, &db, start_index_t3,
                        start_index_t3 + elements_per_thread - 1, &id_sum_t3);
        std::thread worker4 =
            std::thread(thread_func, &db, start_index_t4,
                        start_index_t4 + elements_per_thread - 1, &id_sum_t4);

        worker1.join();
        worker2.join();
        worker3.join();
        worker4.join();

        EXPECT_CALL(db, close()).Times(1).WillOnce(testing::Return(true));
        db.close_db();

        expected_id_sum = id_sum_t1 + id_sum_t2 + id_sum_t3 + id_sum_t4;
        id_sum = db.get_id_sum();
    }

    // check if all created mock_entries got destroied
    EXPECT_EQ(mock_entry::get_constructor_calls(),
              mock_entry::get_destructor_calls());

    // check if sum of ids in the database equals the sum of the ids that are
    // added to the database
    EXPECT_EQ(expected_id_sum, id_sum);
}

TEST(db_test, sqlite) {
    std::string db_path = "./db_tmp.db";
    if (simdal::file_exists(db_path)) {
        std::remove(db_path.c_str());
    }
    EXPECT_FALSE(simdal::file_exists(db_path));

    simdal::db_sqlite db(db_path);
    EXPECT_FALSE(simdal::file_exists(
        db_path));  // database should not be created during instanciation

    db.open_db();
    EXPECT_TRUE(simdal::file_exists(db_path));

    // store some data
    EXPECT_TRUE(db.store(create_entry_ts(&db)));
    EXPECT_TRUE(db.store(create_entry_ts(&db)));

    db.close_db();
    EXPECT_TRUE(simdal::file_exists(db_path));

    if (simdal::file_exists(db_path)) {
        std::remove(db_path.c_str());
    }
}
