/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/csv/csv_file.h"

#include <algorithm>

#include "simdal/utils.h"

namespace simdal {

csv_file::csv_file(const std::string& path, char sep_char)
    : path_(path), sep_char_(sep_char), line_cnt_(0) {}

csv_file::~csv_file() {
    if (is_open()) {
        close();
    }
}

bool csv_file::open() {
    if (stream_.is_open()) {
        LOG_DBG("stream is already open");
        return false;
    }

    stream_.open(path_, std::ios::out | std::ios::in | std::ios::trunc);
    return stream_.is_open();
}

bool csv_file::close() {
    if (!stream_.is_open()) {
        LOG_DBG("stream cannot be closed because it has not been opened yet");
        return false;
    }

    stream_.close();
    return !stream_.is_open();
}

bool csv_file::is_open() const { return stream_.is_open(); }

bool csv_file::write(const std::list<std::string>& elem) {
    std::string line;

    for (const auto& e : elem) {
        line += e;
        if (e != elem.back()) {
            line += sep_char_;
        }
    }
    return write(line);
}

bool csv_file::write(const std::string& line) {
    if (!stream_.is_open()) {
        LOG_DBG("stream is not open");
        return false;
    }

    stream_.clear();                  // reset read and write pointer
    stream_.seekp(0, std::ios::end);  // position write pointer at end of file
    stream_ << line << std::endl;
    stream_.flush();

    line_cnt_ += std::count(line.begin(), line.end(), '\n') + 1;
    return true;
}

bool csv_file::read_line(std::string& line) {
    if (!stream_.is_open()) {
        LOG_DBG("stream is not open");
        return false;
    }

    std::getline(stream_, line);
    return !stream_.eof();
}

bool csv_file::reset_read_pos() {
    if (!stream_.is_open()) {
        LOG_DBG("stream is not open");
        return false;
    }

    stream_.clear();
    stream_.seekg(0, std::ios::beg);
    return true;
}

const std::string& csv_file::get_path() const { return path_; }

bool csv_file::find_line_that_contains(const std::string& search_str,
                                       unsigned int elem_pos,
                                       unsigned int& line_nr, std::string& line,
                                       unsigned int skip_lines) {
    if (!reset_read_pos()) {
        return false;
    }
    line_nr = 0;
    line = "";
    while (line_nr < skip_lines) {
        if (!read_line(line)) {
            return false;
        }
        ++line_nr;
    }

    std::string elem;
    while (read_line(line) &&
           get_line_element(elem_pos, line, elem, sep_char_) &&
           elem != search_str) {
        ++line_nr;
    }

    return elem == search_str;
}

bool csv_file::get_id_of(const std::string& key, const unsigned int key_pos,
                         const unsigned int id_pos, bool& found,
                         std::string& line, unsigned int& id,
                         const unsigned int skip_lines) {
    found = false;
    line = "";
    if (!reset_read_pos()) {
        return false;
    }
    unsigned int line_nr = 0;
    line = "";
    while (line_nr < skip_lines) {
        if (!read_line(line)) {
            return false;
        }
        ++line_nr;
    }

    std::string elem;
    std::string id_str;
    unsigned int max_id = 0;
    while (!found && read_line(line) &&
           get_line_element(key_pos, line, elem, sep_char_)) {
        if (line.empty()) {
            continue;
        }

        if (!get_line_element(id_pos, line, id_str, sep_char_)) {
            return false;
        }

        try {
            id = std::stoi(id_str);
        } catch (...) {
            line = "";
            return false;
        }

        max_id = id > max_id ? id : max_id;

        if (elem == key) {
            found = true;
        }
        ++line_nr;
    }

    if (!found) {
        id = line_nr <= skip_lines ? 1 : max_id + 1;
        line = "";
    }

    return true;
}

bool csv_file::get_line_element(const unsigned int pos, const std::string& line,
                                std::string& elem, const char sep_char) {
    size_t elem_start = 0;
    size_t elem_end = line.find(';', elem_start);
    unsigned int elem_nr = 0;

    while (elem_nr < pos && elem_end != std::string::npos) {
        elem_start = elem_end + 1;
        elem_end = line.find(sep_char, elem_start);
        elem_nr++;
    }

    if (elem_nr == pos) {
        elem_end = elem_end == std::string::npos ? line.size() : elem_end;

        elem = line.substr(elem_start, elem_end - elem_start);
        return true;
    }

    return false;
}

unsigned int csv_file::get_line_cnt() const { return line_cnt_; }

}  // namespace simdal
