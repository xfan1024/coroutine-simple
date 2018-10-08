#include "coroutine.h"

static inline void __set_stack_and_call(struct coroutine * co, void * param, void * stack, unsigned int stack_size, coroutine_function_t entry)
{
    stack = (char*)stack + stack_size;
    __asm__(
        "mov %0, %%rdi\n"   // param 1 = co
        "mov %1, %%rsi\n"   // param 2 = param
        "mov %2, %%rsp\n"   // switch new stack
        "call %P3\n"         // call coroutine_entry
    : "=m"(co), "=m"(param), "=m"(stack)
    : "i"(coroutine_entry)
    );
}
