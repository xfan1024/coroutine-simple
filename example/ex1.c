#include <stdio.h>
#include "coroutine.h"

static void coentry(struct coroutine * co, void * coroutine_param, void * resume_message)
{
    char yield_message[32];
    int i;

    printf("%s: coroutine_param: %s\n", __func__, (char*)coroutine_param);

    for (i = 0; i < 10; i++) {
        printf("%s: resume_message: %s\n", __func__, (char*)resume_message);
        sprintf(yield_message, "this is yield message(%d)", i);
        resume_message = coroutine_yield(co, yield_message);
    }
}

int main()
{
    struct coroutine co;
    static unsigned char co_stack[1024*64];
    char resume_message[32];
    const char * yield_message;
    int i = 0;

    coroutine_init(&co, "this is coroutine parameter", co_stack, sizeof(co_stack), coentry);
    
    while (1)
    {
        sprintf(resume_message, "this is resume message(%d)", i++);
        yield_message = coroutine_resume(&co, resume_message);
        if (coroutine_state(&co) != co_state_suspend) {
            break;
        }
        printf("%s: yield_message: %s\n", __func__, yield_message);
    }

    return 0;
}

