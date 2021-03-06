#include <assertions-test/test.h>
#include <cpp-bash/bash/shell.h>

using namespace std;

tests {
	test_suite("when removing local dependencies") {
		setup(string, test_environment) {
			bash::shell shell;

			shell.exec(R"(
				environment_dir=$(tests/setup_environment.sh)
				cd "$environment_dir"
				./dependencies.sh add git rockerbacon/cpp-benchmark --local-only
				yes | ./dependencies.sh remove git rockerbacon/cpp-benchmark
			)");

			return shell.getvar("environment_dir").get();
		};

		teardown(environment_dir) {
			bash::shell shell;

			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(rm -rf "$environment_dir")").wait();
		};

		test_case("local dependency should be removed from external_dependencies/local_objs") {
			bash::shell shell;

			shell.setvar("environment_dir", (string)*test_environment);
			shell.exec(R"(
				cd "$environment_dir"
				dependency_dir=$(ls external_dependencies/local_objs | grep cpp-benchmark)
				if [ "$dependency_dir" == "" ]; then
					dependency_was_removed=true
				else
					dependency_was_removed=false
				fi
			)");
			auto dependency_was_removed = shell.getvar("dependency_was_removed").get();

			assert(dependency_was_removed, ==, "true");
		};

		test_case("local dependency should be unlisted") {
			bash::shell shell;

			shell.setvar("environment_dir", (string)*test_environment);
			shell.exec(R"(
				cd "$environment_dir"
				dependency_in_list=$(grep cpp-benchmark .assertions/dependency_manager/install.sh)
				if [ "$dependency_in_list" == "" ]; then
					dependency_was_unlisted=true
				else
					dependency_was_unlisted=false
				fi
			)");
			auto dependency_was_unlisted = shell.getvar("dependency_was_unlisted").get();

			assert(dependency_was_unlisted, ==, "true");
		};
	}

	test_suite("when removing dependencies") {
		setup(string, test_environment) {
			bash::shell shell;

			shell.exec(R"(
				environment_dir=$(tests/setup_environment.sh)
				cd "$environment_dir"
				./dependencies.sh add git rockerbacon/cpp-benchmark
				yes | ./dependencies.sh remove git rockerbacon/cpp-benchmark
			)").wait();

			return shell.getvar("environment_dir").get();
		};

		teardown(environment_dir) {
			bash::shell shell;

			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(rm -rf "$environment_dir")").wait();
		};

		test_case("dependency should be removed from external_dependencies/objs") {
			bash::shell shell;

			shell.setvar("environment_dir", (string)*test_environment);
			shell.exec(R"(
				cd "$environment_dir"
				dependency_dir=$(ls external_dependencies/objs | grep cpp-benchmark)
				if [ "$dependency_dir" == "" ]; then
					dependency_was_removed=true
				else
					dependency_was_removed=false
				fi
			)");
			auto dependency_was_removed = shell.getvar("dependency_was_removed").get();

			assert(dependency_was_removed, ==, "true");
		};

		test_case("dependency should be unlisted") {
			bash::shell shell;

			shell.setvar("environment_dir", (string)*test_environment);
			shell.exec(R"(
				cd "$environment_dir"
				dependency_in_list=$(grep cpp-benchmark .assertions/dependency_manager/install.sh)
				if [ "$dependency_in_list" == "" ]; then
					dependency_was_unlisted=true
				else
					dependency_was_unlisted=false
				fi
			)");
			auto dependency_was_unlisted = shell.getvar("dependency_was_unlisted").get();

			assert(dependency_was_unlisted, ==, "true");
		};
	}
}
