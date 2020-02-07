#include <assertions-test/test.h>
#include <cpp-bash/bash/shell.h>

using namespace std;

tests {
	test_suite("when adding a single git dependency") {
		setup(std::string, test_environment) {
			bash::shell shell;
			shell.exec(R"(
				environment_dir=$(./tests/setup_environment.sh)
				cd "$environment_dir"
				./dependencies.sh add git rockerbacon/cpp-benchmark
			)");
			return shell.getvar("environment_dir").get();
		};

		teardown(environment_dir) {
			bash::shell shell;
			shell.setvar("environment_dir", environment_dir);
			shell.exec("rm -rf '$environment_dir'").wait();
		};

		test_case("should create an external dependencies directory") {
			bash::shell shell;
			const string& environment_dir = *test_environment;
			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(
				cd "$environment_dir"
				if [ -d external_dependencies ]; then
					dir_created=true
				else
					dir_created=false
				fi
			)");

			auto dir_created = shell.getvar("dir_created").get();
			assert(dir_created, ==, "true");
		};

		test_case("should create a git directory") {
			bash::shell shell;
			const string& environment_dir = *test_environment;
			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(
				cd "$environment_dir"
				if [ -d external_dependencies/git ]; then
					dir_created=true
				else
					dir_created=false
				fi
			)");

			auto dir_created = shell.getvar("dir_created").get();
			assert(dir_created, ==, "true");
		};

		test_case("should clone the git repository") {
			bash::shell shell;
			const string& environment_dir = *test_environment;
			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(
				cd "$environment_dir"
				if [ -d external_dependencies/git/cpp-benchmark/.git ]; then
					repository_cloned=true
				else
					repository_cloned=false
				fi
			)");

			auto repository_cloned = shell.getvar("repository_cloned").get();

			assert(repository_cloned, ==, "true");
		};
	}
}

