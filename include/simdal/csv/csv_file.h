/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_CSV_CSV_FILE_H_
#define SIMDAL_CSV_CSV_FILE_H_

#include <fstream>
#include <iostream>
#include <list>
#include <string>

namespace simdal {

class csv_file {
  public:
    explicit csv_file(const std::string& path, char sep_char = ';');
    csv_file() = delete;
    csv_file(const csv_file&) = delete;
    virtual ~csv_file();

    bool open();
    bool close();
    bool is_open() const;
    bool write(const std::list<std::string>& elem);
    bool write(const std::string& line);
    bool reset_read_pos();
    bool read_line(std::string& line);
    const std::string& get_path() const;
    bool find_line_that_contains(const std::string& search_str,
                                 unsigned int elem_pos, unsigned int& line_nr,
                                 std::string& line,
                                 unsigned int skip_lines = 0);
    bool get_id_of(const std::string& key, unsigned int key_pos,
                   unsigned int id_pos, bool& found, std::string& line,
                   unsigned int& id, unsigned int skip_lines = 0);
    unsigned int get_line_cnt() const;

    static bool get_line_element(unsigned int pos, const std::string& line,
                                 std::string& elem, char sep_char = ';');

  private:
    std::string path_;
    std::fstream stream_;
    char sep_char_;
    unsigned int line_cnt_;
};

}  // namespace simdal
#endif  // SIMDAL_CSV_CSV_FILE_H_
