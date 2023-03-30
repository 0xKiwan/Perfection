#ifndef _PERFECTION_RESULT_H
#define _PERFECTION_RESULT_H

// Represents a result for a given 'PerfectionLang' function.
typedef enum _perf_e_result_t
{
    PERF_RES_OK,
    PERF_RES_MEMORY_ALLOC_FAIL,
    PERF_RES_LEX_ERROR,
    PERF_RES_PARSE_ERROR
} perf_e_result_t;

typedef int32_t perf_result_t;

#endif // _PERFECTION_RESULT_H



