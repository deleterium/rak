//
// error.c
//
// Copyright 2025 Fábio de Souza Villaça Medeiros
//
// This file is part of the Rak Project.
// For detailed license information, please refer to the LICENSE file
// located in the root directory of this project.
//

#include "rak/error.h"
#include <stdio.h>

void rak_error_init(RakError *err)
{
  err->ok = true;
}

void rak_error_set(RakError *err, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  rak_error_set_with_args(err, fmt, args);
  va_end(args);
}

/** Set error, including arguments.
 *  @param fmt: format string (printf like).
    @param args: arguments to be printed. Must be included in 'fmt' string
 */
void rak_error_set_with_args(RakError *err, const char *fmt, va_list args)
{
  err->ok = false;
  vsnprintf(err->cstr, RAK_ERROR_MAX_LEN, fmt, args);
  err->cstr[RAK_ERROR_MAX_LEN] = '\0';
}

/// Simple error printing. Default output is stderr.
void rak_error_print(RakError *err)
{
  fprintf(stderr, "ERROR: %s\n", err->cstr);
}
