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
#include <string.h>

void rak_error_init(RakError *err)
{
  err->ok = true;
  err->line = 0;
  err->col = 0;
}

void rak_error_set(RakError *err, const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  rak_error_set_with_args(err, fmt, args);
  va_end(args);
}

void rak_error_set_with_args(RakError *err, const char *fmt, va_list args)
{
  err->ok = false;
  vsnprintf(err->cstr, RAK_ERROR_MAX_LEN, fmt, args);
  err->cstr[RAK_ERROR_MAX_LEN] = '\0';
}

void rak_error_set_line_col(RakError *err, int line, int col)
{
  // Do not overwrite previous error info
  if (err->line == 0) err->line = line;
  if (err->col == 0) err->col = col;
}

void rak_error_print(RakError *err)
{
  fprintf(stderr, "ERROR: %s\n", err->cstr);
}

void get_line_and_len(char * sourceCode, int line, char **charLine, int *len)
{
  int currLine = 0;
  int curr = 0;
  do
  {
    *charLine = sourceCode + curr;
    char *e = strchr(*charLine, '\n');
    if (e == NULL)
    {
      e = strchr(*charLine, '\0');
      if (e == NULL) return;
    }
    *len = (int)(e - *charLine);
    currLine++;
    curr += *len + 1;
  } while (currLine < line);
  return;
}

void rak_error_print_compile(RakError *err, char *sourceCode)
{
  if (!err->line)
  {
    rak_error_print(err);
    return;
  }

  char *charLine = NULL;
  int len = 0;

  get_line_and_len(sourceCode, err->line, &charLine, &len);

  fprintf(stderr, "ERROR at %d:%d - %s\n", err->line, err->col, err->cstr);
  fprintf(stderr, " |%.*s\n", len, charLine);
  fprintf(stderr, " |%*s^\n", err->col - 1, "");
  return;
}