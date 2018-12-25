#include <string.h>
#include <assert.h>
#include "coroutine.h"

static void coroutine_entry(struct coroutine * co, void * coroutine_param)
{
    void * resume_message;
    co->state = co_state_running;
    resume_message = coroutine_yield(co, NULL);
    co->entry(co, coroutine_param, resume_message);
    co->state = co_state_dead;
    coroutine_yield(co, NULL);
}

#include COROUTINE_MACH

const char* coroutine_state_to_string(enum coroutine_state state)
{
    static const char * strs[] = {
        "co_state_suspend",
        "co_state_running",
        "co_state_dead",
        "co_state_unknown",
    };
    if (state < co_state_max) {
        return strs[state];
    } else {
        return strs[co_state_max];
    }
}

void coroutine_init(struct coroutine * co, void * param, void * stack, unsigned int stack_size, coroutine_function_t entry)
{
    memset(co, 0, sizeof(*co));
    co->state = co_state_suspend;
    co->entry = entry;
    if (!setjmp(co->caller_jmpbuf)) {
        __set_stack_and_call(co, param, stack, stack_size);
    }
}

void * coroutine_yield(struct coroutine * co, void * yield_message)
{
    assert(co->state != co_state_suspend);
    
    if (co->state == co_state_running) {
        co->state = co_state_suspend;
    }
    co->exchange_value = yield_message;
    if (!setjmp(co->coroutine_jmpbuf)) {
        longjmp(co->caller_jmpbuf, 1);
    }
    return co->exchange_value;
}

void * coroutine_resume(struct coroutine * co, void * resume_message)
{
    assert(co->state == co_state_suspend);

    co->state = co_state_running;
    co->exchange_value = resume_message;
    if (!setjmp(co->caller_jmpbuf)) {
        longjmp(co->coroutine_jmpbuf, 1);
    }
    return co->exchange_value;
}
