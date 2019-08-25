#include <bench_util/bench_util.h>
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

namespace {

TEST(bench_util, basics) {
	bench::suite suite;
	suite.title("test title");
	suite.benchmark("test1 blee", []() { std::this_thread::sleep_for(1s); });
	suite.benchmark("test2 blee", []() { std::this_thread::sleep_for(2s); });
	suite.benchmark("test2 blee", []() { std::this_thread::sleep_for(0.5s); });
	suite.print();
	suite.clear();
}

} // namespace

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
