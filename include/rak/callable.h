//
// callable.h
//
// Copyright 2025 Fábio de Souza Villaça Medeiros
//
// This file is part of the Rak Project.
// For detailed license information, please refer to the LICENSE file
// located in the root directory of this project.
//

#ifndef RAK_CALLABLE_H
#define RAK_CALLABLE_H

#include "string.h"

/// Types of functions that are Callables.
typedef enum
{
  RAK_CALLABLE_TYPE_FUNCTION,
  RAK_CALLABLE_TYPE_NATIVE_FUNCTION
} RakCallableType;

typedef struct
{
  RakObject      obj;
  RakString     *name;
  int            arity;
  RakSlice(int)  inouts;
} RakCallable;

void rak_callable_init(RakCallable *callable, RakString *name, int arity,
  RakError *err);
void rak_callable_deinit(RakCallable *callable);
void rak_callable_append_inout_param(RakCallable *callable, int idx, RakError *err);

#endif // RAK_CALLABLE_H
