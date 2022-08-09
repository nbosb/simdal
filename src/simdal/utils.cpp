/******************************************************************************
 *                                                                            *
 * Copyright 2022 Nils Bosbach                                                *
 *                                                                            *
 * This software is licensed under the Apache-2.0 license found in the        *
 * LICENSE file at the root directory of this source tree.                    *
 *                                                                            *
 ******************************************************************************/

#include "simdal/utils.h"

#include <cstdarg>

namespace simdal {

const std::chrono::steady_clock::time_point prog_start_time =
    std::chrono::steady_clock::now();

void standard_log(const char* file, const int line, const char* format, ...);
void set_custom_log_function(log_fcn_t log_fcn);

log_fcn_t log_fcn_ptr = &standard_log;

void standard_log(const char* file, const int line, const char* format, ...) {
    va_list args;
    va_start(args, format);

    printf("%s:%d - ", file, line);
    vprintf(format, args);
    printf("\n");

    va_end(args);
}

void set_custom_log_function(log_fcn_t log_fcn) { log_fcn_ptr = log_fcn; }

}  // namespace simdal
