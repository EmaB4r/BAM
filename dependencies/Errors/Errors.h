#ifndef ERRORS_H
#define ERRORS_H
#include <stdio.h>
#include <stdlib.h>

#define ANSI_RESET "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_RED "\033[31m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_GREEN "\033[32m"


typedef struct Option Option_t;
typedef struct Result Result_t;

typedef struct Option{
    enum {Some_opt, None_opt} type;
    void* content;
    void* (*fn_unwrap)(Option_t self);
    int (*fn_isSome)(Option_t self);
}Option_t;

Option_t Option_wrap(void* content, int type);

typedef struct Result{
    enum {Ok_res, Err_res} type;
    void* content;
    void* (*fn_unwrap)(Result_t self);
    int (*fn_isOk)(Result_t self);
}Result_t;

Result_t Result_wrap(void* content, int type);

#define panic(FORMAT, ...) do{\
    printf(ANSI_RED ANSI_BOLD"Panicked in %s at line %d" ANSI_YELLOW "\nError msg: " FORMAT ANSI_RESET,__FILE__,__LINE__, ##__VA_ARGS__); \
    exit(1);\
}while(0)

#define Result_unwrap_or_panic(RESULT, MSG)({\
    if (!result.fn_isOk(RESULT))\
        panic(MSG);\
    result.fn_unwrap(RESULT);\
})


#define Option_unwrap_or_panic(OPTION, MSG)({\
    if (!option.fn_isSome(OPTION))\
        panic(MSG);\
    option.fn_unwrap(OPTION);\
})


#endif