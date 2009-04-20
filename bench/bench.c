// bench.c
//
// Test fixed-point math library (precision, speed)
//

// default include header files
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/resource.h>

// define precision function pointer
typedef void(*precision_function_t) (
    unsigned int _res,
    float * _rmse);

// define benchmark function pointer
typedef void(*benchmark_function_t) (
    struct rusage * _start,
    struct rusage * _finish,
    unsigned long int * _num_trials);

// define basic structure for storing results
typedef struct {
    // precision
    precision_function_t precision_api;
    float rmse;

    // speed
    benchmark_function_t benchmark_api;
    unsigned long int num_trials;
    float extime;
    float rate;
    float cycles_per_trial;
} bench_t;

double calculate_execution_time(struct rusage _start, struct rusage _finish);

// 
#include "bench/log2_benchmark.h"
#include "bench/sqrt_benchmark.h"
#include "bench/sin_benchmark.h"

int main() {
    //
    struct rusage t0;
    struct rusage t1;
    unsigned long int num_trials = 100000;
    double extime;
    unsigned int resolution = 256;
    float rmse;
     
    // run benchmarks
    precision_log2(resolution,&rmse);
    benchmark_log2(&t0,&t1,&num_trials);

    // compile results
    extime = calculate_execution_time(t0,t1);

    // print results
    printf("    %-20s : %12.6f s : %12.6f\n", "q32_log2", extime, rmse);

   
    // run benchmarks
    precision_sqrt(resolution,&rmse);
    benchmark_sqrt(&t0,&t1,&num_trials);

    // compile results
    extime = calculate_execution_time(t0,t1);

    // print results
    printf("    %-20s : %12.6f s : %12.6f\n", "q32_sqrt", extime, rmse);


   
    // run benchmarks
    precision_sin(resolution,&rmse);
    benchmark_sin(&t0,&t1,&num_trials);

    // compile results
    extime = calculate_execution_time(t0,t1);

    // print results
    printf("    %-20s : %12.6f s : %12.6f\n", "q32_sin", extime, rmse);

    printf("done.\n");

    return 0;
}

double calculate_execution_time(struct rusage _start, struct rusage _finish)
{
    return _finish.ru_utime.tv_sec - _start.ru_utime.tv_sec
        + 1e-6*(_finish.ru_utime.tv_usec - _start.ru_utime.tv_usec)
        + _finish.ru_stime.tv_sec - _start.ru_stime.tv_sec
        + 1e-6*(_finish.ru_stime.tv_usec - _start.ru_stime.tv_usec);
}


