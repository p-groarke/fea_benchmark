#include <bench_util/bench_util.h>
#include <chrono>
#include <cstdio>
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

namespace {

TEST(bench_util, basics) {
	bench::suite suite;
	suite.title("suite test");
	suite.benchmark("test1 blee", []() { std::this_thread::sleep_for(0.1s); });
	suite.benchmark("test2 blee", []() { std::this_thread::sleep_for(0.2s); });
	suite.benchmark("test2 blee", []() { std::this_thread::sleep_for(0.5s); });
	suite.print();
	suite.clear();

	suite.title("suite averages");
	suite.benchmark(
			"test1 blee", []() { std::this_thread::sleep_for(0.2s); }, 2);
	suite.benchmark(
			"test2 blee", []() { std::this_thread::sleep_for(0.1s); }, 4);
	suite.benchmark(
			"test2 blee", []() { std::this_thread::sleep_for(0.05s); }, 10);
	suite.print();
	suite.clear();

	size_t in_between = 0;
	suite.title("suite averages in-between");
	suite.benchmark(
			"test1 blee", []() { std::this_thread::sleep_for(0.2s); }, 1,
			[&]() { ++in_between; });
	suite.benchmark(
			"test1 blee", []() { std::this_thread::sleep_for(0.2s); }, 2,
			[&]() { ++in_between; });
	suite.benchmark(
			"test2 blee", []() { std::this_thread::sleep_for(0.1s); }, 4,
			[&]() { ++in_between; });
	suite.benchmark(
			"test2 blee", []() { std::this_thread::sleep_for(0.05s); }, 10,
			[&]() { ++in_between; });

	EXPECT_EQ(in_between, 17);
	suite.print();
	suite.clear();
}

} // namespace

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
