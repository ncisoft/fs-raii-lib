#pragma once

typedef struct coroutine coroutine_t ;
typedef coroutine_t co_t;

typedef enum {

    COROUTINE_RUNNING,
    COROUTINE_SUSPENDED,
    COROUTINE_NORMAL,
    COROUTINE_DEAD,

} COROUTINE_STATUS;

extern  coroutine_t *coroutine_create();
extern void cortoutine_resume(coroutine_t *co);
extern void cortoutine_yield();
extern coroutine_t *coroutine_running ();
extern coroutine_t *coroutine_status (coroutine_t *co);

