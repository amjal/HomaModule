#include "bench_harness.h"
#include <iostream>
#include <string>
#include <cstring>

/*
 * Re-define the structs that are refernced by the test objects 
 * and defined by kselftest_karness main. Becaues for the benchmark
 * program we are not linking with the kselftest_harness main.
 */

/* Contains all the information for test execution and status checking. */
struct __test_metadata {
	const char *name;
	void (*fn)(struct __test_metadata *);
	int termsig;
	int passed;
	int trigger; /* extra handler after the evaluation */
	unsigned char step;
	int no_print; /* manual trigger when TH_LOG_STREAM is not available */
	struct __test_metadata *prev, *next;
};

/* Storage for the (global) tests to be run. */
struct __test_metadata *__test_list;
struct __test_metadata *__current_test;
unsigned int __test_count;
unsigned int __fixture_count;

std::string helpMessage = 
    "Help message\n";

int main(int argc, char **argv){
    int i;
    for(i = 1; i < argc; i ++){
        if ((strcmp(argv[i], "-h") == 0) ||
            (strcmp(argv[i], "--help") == 0)) {
            std::cout<<helpMessage<<'\n';
            return 0;
        } else if (argv[i][0] == '-') {
            printf("Unknown option %s; type '%s --help' for help\n",
                argv[i], argv[0]);
            return 1;	
        } else {
            break;
        }  
    }
    bench_harness_run(argc-i, argv+i);
}