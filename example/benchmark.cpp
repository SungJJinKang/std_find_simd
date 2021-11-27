#include <benchmark/benchmark.h>
#include "random"

#include "../std_vector_find_simd.h"


static void Scalar(benchmark::State& state)
{
	std::vector<long long> a;
	a.reserve(1001);
	for (unsigned long long i = 0; i <= 1000; i++)
	{
		a.push_back(i);
	}
    for (auto _ : state)
    {
		for (long long i = 0; i < 1000; i++)
		{
			volatile auto iter = fast_vector_find::find_simd(a, (long long)i);
		}
    }
}
// Register the function as a benchmark
BENCHMARK(Scalar);

// Define another benchmark
static void Simd(benchmark::State& state)
{
	std::vector<long long> a;
	a.reserve(1001);
	for (unsigned long long i = 0; i <= 1000; i++)
	{
		a.push_back(i);
	}
    for (auto _ : state)
    {
		for (long long i = 0; i < 1000; i++)
		{
			volatile auto iter = fast_vector_find::find_simd(a, (long long)i);
		}
    }

}
BENCHMARK(Simd);

BENCHMARK_MAIN();