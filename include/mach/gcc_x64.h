#include "coroutine.h"

static inline void __set_stack_and_call(struct coroutine * co, void * param, void * stack, unsigned int stack_size)
{
    stack = (char*)stack + stack_size;
    __asm__(
        "mov %[p1], %%rdi\n"        // param 1 = co
        "mov %[p2], %%rsi\n"        // param 2 = param
        "mov %[stack], %%rsp\n"     // switch new stack
        "call coroutine_entry\n"    // call coroutine_entry
    :
    : [p1]"m"(co), [p2]"m"(param), [stack]"m"(stack)
    );
}
