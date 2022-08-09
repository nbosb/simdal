/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/csv/csv_file.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>

#include "simdal/utils.h"

TEST(csv_test, open_close) {
    std::string db_path = "./tmp_file.csv";
    if (simdal::file_exists(db_path)) {
        std::remove(db_path.c_str());
    }
    EXPECT_FALSE(simdal::file_exists(db_path));

    std::list<std::string> elems = {"zero", "one", "two", "three", "four"};
    std::string line = "five;six;seven;eight;nine";
    std::string line2 = "ten;eleven;twelve;thirteen;fourteen";

    simdal::csv_file db_file(db_path);
    EXPECT_FALSE(simdal::file_exists(
        db_path));  // database should not be created during instanciation

    EXPECT_FALSE(db_file.write(elems));
    EXPECT_FALSE(db_file.write(line));

    EXPECT_TRUE(db_file.open());
    EXPECT_TRUE(simdal::file_exists(db_path));

    EXPECT_TRUE(db_file.write(elems));
    EXPECT_TRUE(db_file.write(line));

    std::string tmp_line;
    unsigned int line_nr = 12;
    EXPECT_TRUE(
        db_file.find_line_that_contains("zero", 0, line_nr, tmp_line, 0));
    EXPECT_EQ(line_nr, 0);
    EXPECT_EQ(tmp_line, std::string("zero;one;two;three;four"));

    EXPECT_FALSE(db_file.find_line_that_contains("zero", 0, line_nr, tmp_line,
                                                 1));  // skip first line
    EXPECT_FALSE(db_file.find_line_that_contains(
        "zero", 1, line_nr, tmp_line, 0));  // search at the wrong position
    EXPECT_FALSE(db_file.find_line_that_contains(
        "zero", 5, line_nr, tmp_line,
        0));  // search at a none-existing position

    EXPECT_TRUE(
        db_file.find_line_that_contains("four", 4, line_nr, tmp_line, 0));
    EXPECT_EQ(line_nr, 0);
    EXPECT_EQ(tmp_line, std::string("zero;one;two;three;four"));

    EXPECT_TRUE(
        db_file.find_line_that_contains("five", 0, line_nr, tmp_line, 0));
    EXPECT_EQ(line_nr, 1);
    EXPECT_EQ(tmp_line, line);

    EXPECT_TRUE(
        db_file.find_line_that_contains("five", 0, line_nr, tmp_line, 1));
    EXPECT_EQ(line_nr, 1);
    EXPECT_EQ(tmp_line, line);

    EXPECT_TRUE(
        db_file.find_line_that_contains("six", 1, line_nr, tmp_line, 0));
    EXPECT_EQ(line_nr, 1);
    EXPECT_EQ(tmp_line, line);

    EXPECT_TRUE(db_file.write(line2));  // insert line

    EXPECT_TRUE(
        db_file.find_line_that_contains("seven", 2, line_nr, tmp_line, 0));
    EXPECT_EQ(line_nr, 1);
    EXPECT_EQ(tmp_line, line);

    EXPECT_TRUE(
        db_file.find_line_that_contains("nine", 4, line_nr, tmp_line, 0));
    EXPECT_EQ(line_nr, 1);
    EXPECT_EQ(tmp_line, line);

    EXPECT_TRUE(
        db_file.find_line_that_contains("thirteen", 3, line_nr, tmp_line, 0));
    EXPECT_EQ(line_nr, 2);
    EXPECT_EQ(tmp_line, line2);

    EXPECT_TRUE(db_file.close());
    EXPECT_TRUE(simdal::file_exists(db_path));

    EXPECT_FALSE(db_file.close());

    EXPECT_FALSE(db_file.write(elems));
    EXPECT_FALSE(db_file.write(line));
    EXPECT_FALSE(
        db_file.find_line_that_contains("seven", 2, line_nr, tmp_line, 0));

    if (simdal::file_exists(db_path)) {
        std::remove(db_path.c_str());
    }
}

TEST(csv_test, find_id) {
    std::string db_path = "./tmp_file.csv";
    if (simdal::file_exists(db_path)) {
        std::remove(db_path.c_str());
    }
    EXPECT_FALSE(simdal::file_exists(db_path));

    std::string line_0 = "id0;key0;key1;id1";
    std::string line_1 = "0;apple;cat;20";
    std::string line_2 = "1;banana;dog;21";
    simdal::csv_file db_file(db_path);

    EXPECT_TRUE(db_file.open());

    std::string line = "content";
    unsigned int id = 234234;
    bool found = true;

    EXPECT_TRUE(db_file.get_id_of("apple", 1, 0, found, line, id, 0));
    EXPECT_FALSE(found);
    EXPECT_EQ(id, 1);
    EXPECT_TRUE(line.empty());

    EXPECT_TRUE(db_file.write(line_0));

    EXPECT_FALSE(db_file.get_id_of(
        "apple", 1, 0, found, line, id,
        0));  // don't skip header -> should result in an error
    EXPECT_TRUE(db_file.get_id_of("apple", 1, 0, found, line, id, 1));
    EXPECT_FALSE(found);
    EXPECT_EQ(id, 1);
    EXPECT_TRUE(line.empty());

    EXPECT_TRUE(db_file.write(line_1));

    EXPECT_FALSE(db_file.get_id_of(
        "apple", 1, 0, found, line, id,
        0));  // don't skip header -> should result in an error
    EXPECT_TRUE(db_file.get_id_of("apple", 1, 0, found, line, id, 1));
    EXPECT_TRUE(found);
    EXPECT_EQ(id, 0);
    EXPECT_EQ(line, line_1);

    EXPECT_TRUE(db_file.get_id_of("cat", 2, 3, found, line, id, 1));
    EXPECT_TRUE(found);
    EXPECT_EQ(id, 20);
    EXPECT_EQ(line, line_1);

    EXPECT_TRUE(db_file.get_id_of("rabbit", 2, 3, found, line, id, 1));
    EXPECT_FALSE(found);
    EXPECT_EQ(id, 21);
    EXPECT_TRUE(line.empty());

    EXPECT_TRUE(db_file.get_id_of("rabbit", 2, 0, found, line, id, 1));
    EXPECT_FALSE(found);
    EXPECT_EQ(id, 1);
    EXPECT_TRUE(line.empty());

    EXPECT_TRUE(db_file.write(line_2));

    EXPECT_FALSE(db_file.get_id_of(
        "apple", 1, 0, found, line, id,
        0));  // don't skip header -> should result in an error
    EXPECT_TRUE(db_file.get_id_of("apple", 1, 0, found, line, id, 1));
    EXPECT_TRUE(found);
    EXPECT_EQ(id, 0);
    EXPECT_EQ(line, line_1);

    EXPECT_TRUE(db_file.get_id_of("cat", 2, 3, found, line, id, 1));
    EXPECT_TRUE(found);
    EXPECT_EQ(id, 20);
    EXPECT_EQ(line, line_1);

    EXPECT_TRUE(db_file.get_id_of("rabbit", 2, 3, found, line, id, 1));
    EXPECT_FALSE(found);
    EXPECT_EQ(id, 22);
    EXPECT_TRUE(line.empty());

    EXPECT_TRUE(db_file.get_id_of("rabbit", 2, 0, found, line, id, 1));
    EXPECT_FALSE(found);
    EXPECT_EQ(id, 2);
    EXPECT_TRUE(line.empty());

    EXPECT_TRUE(db_file.get_id_of("banana", 1, 0, found, line, id, 1));
    EXPECT_TRUE(found);
    EXPECT_EQ(id, 1);
    EXPECT_EQ(line, line_2);

    EXPECT_TRUE(db_file.close());

    if (simdal::file_exists(db_path)) {
        std::remove(db_path.c_str());
    }
}
