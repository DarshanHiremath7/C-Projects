#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,      // e_success = 0, Indicates a successful operation
    e_failure       // e_failure = 1, Indicates a failed operation
} Status;

typedef enum
{
    e_encode,       // e_encode = 0, Operation is encoding
    e_decode,       // e_decode = 1, Operation is decoding
    e_unsupported   // e_unsupported = 2, Operation is unsupported
} OperationType;

#endif
