/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#ifndef SIMDAL_UTILS_H_
#define SIMDAL_UTILS_H_

#include <chrono>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>

#ifndef SIMDAL_DISABLE_LOG
#define LOG_DBG(...)                                             \
    do {                                                         \
        (*simdal::log_fcn_ptr)(__FILE__, __LINE__, __VA_ARGS__); \
    } while (false)
#else
#define LOG_DBG(...)
#endif

namespace simdal {

typedef void (*log_fcn_t)(const char*, const int, const char*, ...);

extern const std::chrono::steady_clock::time_point prog_start_time;
extern log_fcn_t log_fcn_ptr;

extern void set_custom_log_function(log_fcn_t log_fcn);

inline bool file_exists(const std::string& path) {
    std::ifstream f(path.c_str());
    return f.good();
}

inline int64_t get_file_size(const std::string& path) {
    if (!file_exists(path)) {
        return 0;
    }

    FILE* p_file = fopen(path.c_str(), "rb");
    fseek(p_file, 0, SEEK_END);
    int64_t size = ftell(p_file);
    fclose(p_file);
    return size;
}

inline std::string format_filesize(uint64_t bytes) {
    float kb = bytes / 1024.0F;
    float mb = kb / 1024.0F;
    float gb = mb / 1024.0F;

    std::string return_val;

    if (gb >= 1.0) {
        return_val = std::to_string(gb).append(" GiB");
    } else if (mb >= 1.0) {
        return_val = std::to_string(mb).append(" MiB");
    } else if (kb >= 1.0) {
        return_val = std::to_string(kb).append(" KiB");
    } else {
        return_val = std::to_string(bytes).append(" Bytes");
    }
    return return_val;
}

inline std::string append_before_dot(const std::string& str,
                                     const std::string& append) {
    std::size_t dot_pos = str.find_last_of('.');

    if (dot_pos == std::string::npos) {
        return str + append;
    }
    return str.substr(0, dot_pos).append(append).append(str.substr(dot_pos));
}

inline std::string get_time_now_str() {
    auto now_chrono = std::chrono::steady_clock::now();
    auto usec = std::chrono::duration_cast<std::chrono::microseconds>(
        now_chrono - prog_start_time);
    std::ostringstream oss;

    oss << usec.count();
    return oss.str();
}

}  // namespace simdal
#endif  // SIMDAL_UTILS_H_
