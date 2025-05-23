//
// fiber.h
//
// Copyright 2025 Fábio de Souza Villaça Medeiros
//
// This file is part of the Rak Project.
// For detailed license information, please refer to the LICENSE file
// located in the root directory of this project.
//

#ifndef RAK_FIBER_H
#define RAK_FIBER_H

#include "array.h"
#include "closure.h"
#include "stack.h"

#define RAK_FIBER_VSTK_DEFAULT_SIZE (1024)
#define RAK_FIBER_CSTK_DEFAULT_SIZE (128)

typedef enum
{
  RAK_FIBER_STATUS_SUSPENDED,
  RAK_FIBER_STATUS_RUNNING,
  RAK_FIBER_STATUS_DONE
} RakFiberStatus;

typedef struct
{
  RakClosure *cl;
  union
  {
    uint32_t *ip;
    int       state;
  };
  RakValue *slots;
} RakCallFrame;

typedef struct RakFiber
{
  RakObject               obj;
  RakFiberStatus          status;
  RakArray               *globals;
  RakStack(RakValue)      vstk;
  RakStack(RakCallFrame)  cstk;
} RakFiber;

void rak_fiber_init(RakFiber *fiber, RakArray *globals, int vstkSize, int cstkSize,
  RakClosure *cl, uint8_t nargs, RakValue *args, RakError *err);
void rak_fiber_deinit(RakFiber *fiber);
RakFiber *rak_fiber_new(RakArray *globals, int vstkSize, int cstkSize,
  RakClosure *cl, uint8_t nargs, RakValue *args, RakError *err);
void rak_fiber_free(RakFiber *fiber);
void rak_fiber_release(RakFiber *fiber);
void rak_fiber_run(RakFiber *fiber, RakError *err);
void rak_fiber_resume(RakFiber *fiber, RakError *err);

#endif // RAK_FIBER_H
