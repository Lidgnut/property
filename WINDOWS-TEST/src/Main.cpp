
#include <benchmark/benchmark.h>
#include <cstdlib>
#include <vector>

#include "inckude/PropertyHolder.h"

namespace mwaack
{
	struct Property : public I_Property
	{
		using I_Property::castTo;

		Property() = default;
		Property(const Property&) = default;
		Property& operator=(const Property&) = default;
		Property(Property&&) = default;
		Property& operator=(Property&&) = default;
		virtual ~Property() = default;

		int a = 0;
		std::array<uint8_t, 5> arr;
	};

	struct BigProperty : public I_Property
	{
		using I_Property::castTo;

		BigProperty() = default;
		BigProperty(const BigProperty&) = default;
		BigProperty& operator=(const BigProperty&) = default;
		BigProperty(BigProperty&&) = default;
		BigProperty& operator=(BigProperty&&) = default;
		virtual ~BigProperty() = default;

		int a = 0;
		std::array<uint8_t, 1024> arr;
	};
}


static void FastProp(benchmark::State& state)
{
	std::vector<mwaack::PropertyHolder<20>> vec;
	for (int i = 0; i < 1'000'000; ++i)
	{
		vec.emplace_back(mwaack::Property(), mwaack::BigProperty());
	}
	for (auto _ : state)
	{
		for (const auto& val : vec)
		{
			int i = val.cfastProp().castTo<mwaack::Property>().a;
			benchmark::DoNotOptimize(i);
		}
	}
}
// Register the function as a benchmark
BENCHMARK(FastProp);


static void SlowProp(benchmark::State& state)
{
	std::vector<mwaack::PropertyHolder<20>> vec;
	for (int i = 0; i < 1'000'000; ++i)
	{
		vec.emplace_back(mwaack::Property(), mwaack::BigProperty());
	}
	for (auto _ : state)
	{
		for (const auto& val : vec)
		{
			int i = val.cslowProp().castTo<mwaack::BigProperty>().a;
			benchmark::DoNotOptimize(i);
		}
	}
}
// Register the function as a benchmark
BENCHMARK(SlowProp);

int main(int argc, char** argv)
{
	::benchmark::Initialize(&argc, argv);
	if (::benchmark::ReportUnrecognizedArguments(argc, argv))
		return 1;
	::benchmark::RunSpecifiedBenchmarks();
	std::system("pause");
}