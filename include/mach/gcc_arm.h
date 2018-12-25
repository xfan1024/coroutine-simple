#include "coroutine.h"

static inline void __set_stack_and_call(struct coroutine * co, void * param, void * stack, unsigned int stack_size)
{
    stack = (char*)stack + stack_size;
    __asm__(
        "ldr r0, %[p1]\n"           // param 1 = co
        "ldr r1, %[p2]\n"           // param 2 = param
        "ldr sp, %[stack]\n"        // switch new stack
        "b coroutine_entry\n"       // call coroutine_entry
    :
    : [p1]"m"(co), [p2]"m"(param), [stack]"m"(stack)
    );
}
