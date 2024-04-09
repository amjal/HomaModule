#include <benchmark/benchmark.h>

#define FIXTURE_DATA(datatype_name) struct _test_data_##datatype_name

#define BENCH(fixture_name, bench_name) \
    extern "C" { \
        extern FIXTURE_DATA(fixture_name) self_##fixture_name##_##bench_name; \
        extern void bench##_##fixture_name##_##bench_name(void*, \
            FIXTURE_DATA(fixture_name)*); \
        extern void bench_setup##_##fixture_name##_##bench_name( \
            FIXTURE_DATA(fixture_name)*); \
        extern void fixture_name##_teardown(void *, \
            FIXTURE_DATA(fixture_name)*); \
    } \
    static void wrapper_##fixture_name##_##bench_name( \
        benchmark::State& state)\
    { \
        bench_setup##_##fixture_name##_##bench_name( \
            &self_##fixture_name##_##bench_name); \
        while(state.KeepRunning()){ \
            bench##_##fixture_name##_##bench_name(NULL, \
            &self_##fixture_name##_##bench_name); \
        } \
        fixture_name##_teardown(NULL, \
        &self_##fixture_name##_##bench_name); \
    } \
    BENCHMARK(wrapper##_##fixture_name##_##bench_name);  \
    static struct __bench_metadata \
                _##fixture_name##_##bench_name##_object = { \
        name: #fixture_name "." #bench_name, \
        fn: &wrapper##_##fixture_name##_##bench_name \
    };\
    static void __attribute__((constructor)) \
            _register_##fixture_name##_##bench_name(void) \
    { \
        __register_bench(&_##fixture_name##_##bench_name##_object); \
    }

struct __bench_metadata {
    const char *name;
    void (*fn)(benchmark::State&);
    struct __bench_metadata *prev, *next;
};

extern struct __bench_metadata *__bench_list;
extern struct __bench_metadata *__current_list;
extern unsigned int __bench_count;
extern int __constructor_order;


#define _CONSTRUCTOR_ORDER_FORWARD 1
#define _CONSTRUCTOR_ORDER_BACKWARD -1

static inline void __register_bench(struct __bench_metadata *t)
{
    __bench_count ++;
    if (__bench_list == NULL){
        __bench_list = t;
        t->next = NULL;
        t->prev = t;
    } else if(__constructor_order == _CONSTRUCTOR_ORDER_FORWARD){
        t->next = NULL;
        t->prev = __bench_list->prev;
        t->prev->next = t;
        __bench_list->prev = t;
    } else {
        t->next = __bench_list;
        t->next->prev = t;
        t->prev = t;
        __bench_list = t;
    }
}

#ifndef BENCH_HARNESS_NOT_MAIN

struct __bench_metadata *__bench_list;
struct __bench_metadata *__current_list;
unsigned int __bench_count;
int __constructor_order;

static void bench_harness_run(int argc, char **argv){
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
}

#endif /* BENCH_HARNESS_NOT_MAIN */

static void __attribute__((constructor)) __constructor_order_first(void)
{
	if (!__constructor_order)
		__constructor_order = _CONSTRUCTOR_ORDER_FORWARD;
}

static void __attribute__((constructor)) __constructor_order_second(void)
{
	if (!__constructor_order)
		__constructor_order = _CONSTRUCTOR_ORDER_BACKWARD;
}
