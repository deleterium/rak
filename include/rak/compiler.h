//
// compiler.h
//
// Copyright 2025 Fábio de Souza Villaça Medeiros
//
// This file is part of the Rak Project.
// For detailed license information, please refer to the LICENSE file
// located in the root directory of this project.
//

#ifndef RAK_COMPILER_H
#define RAK_COMPILER_H

#include "closure.h"

#define RAK_COMPILER_MAX_SYMBOLS (UINT8_MAX + 1)

typedef enum
{
  START,
  NEXT,
  ELSE,
  END
} LabelType;

typedef enum
{
  RUI_EMPTY,
  RUI_LOOP,
  RUI_WHILE,
  RUI_IF,
  RUI_AND,
  RUI_OR
} Initiator;

/**
 * @brief Represents label
 * 
 * To easy debug, items are separated.
 */
typedef struct {
    int index; ///< incremental
    Initiator initiator; ///< type of label initiator
    LabelType type; ///< Label type (start, next or end)
} Label;

/**
 * @brief Represents label tuple (address and label details)
 * 
 */
typedef struct {
    int address;
    Label label;
} LabelTuple;

RakClosure *rak_compile(RakString *file, RakString *source, RakError *err);

#endif // RAK_COMPILER_H
