#ifndef __coroutine_h__
#define __coroutine_h__
#include <setjmp.h>


enum coroutine_state {
    co_state_suspend,
    co_state_running,
    co_state_dead,
    co_state_max,
};


struct coroutine;
typedef void (*coroutine_function_t)(struct coroutine * co, void * coroutine_param, void * yield_message);

struct coroutine {
    jmp_buf                 caller_jmpbuf;
    jmp_buf                 coroutine_jmpbuf;
    coroutine_function_t    entry;
    void * volatile         exchange_value;
    enum coroutine_state    state;
};

const char* coroutine_state_to_string(enum coroutine_state state);

void coroutine_init(struct coroutine * co, void * param, void * stack, unsigned int stack_size, coroutine_function_t entry);
void * coroutine_yield(struct coroutine * co, void * yield_message);
void * coroutine_resume(struct coroutine * co, void * resume_message);
static inline enum coroutine_state coroutine_state(struct coroutine * co) { return co->state; }

#endif
