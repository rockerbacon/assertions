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
			shell.exec(R"(rm -rf "$environment_dir")").wait();
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

		test_case("should link source code from the cloned repository to external_dependencies/objs") {
			bash::shell shell;
			const string& environment_dir = *test_environment;
			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(
				cd "$environment_dir"
				files_in_cloned_repo=$(ls external_dependencies/git/cpp-benchmark/src/objs)
				expected_links_count=0
				actual_links_count=0
				for source_file in $files_in_cloned_repo; do
					link="external_dependencies/objs/cpp-benchmark/$source_file"
					if [ -L "$link" ]; then
						file_is_linked=$(readlink -f "$link" | grep -o "$source_file")
						if [ "$file_is_linked" != "" ]; then
							actual_links_count=`expr $actual_links_count + 1`
						fi
					fi
					expected_links_count=`expr $expected_links_count + 1`
				done
			)");

			auto expected_links_count = shell.getvar("expected_links_count").get();
			auto actual_links_count = shell.getvar("actual_links_count").get();

			assert(actual_links_count, ==, expected_links_count);
		};
	}

	test_suite("when adding a local-only dependency") {
		setup(string, test_environment) {
			bash::shell shell;
			shell.exec(R"(
				environment_dir=$(./tests/setup_environment.sh)
				cd "$environment_dir"
				./dependencies.sh add git rockerbacon/cpp-benchmark --local-only
			)");
			return shell.getvar("environment_dir").get();
		};

		teardown(environment_dir) {
			bash::shell shell;
			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(rm -rf "$environment_dir")");
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

		test_case("should link source code from the cloned repository to external_dependencies/local_objs") {
			bash::shell shell;
			const string& environment_dir = *test_environment;
			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(
				cd "$environment_dir"
				files_in_cloned_repo=$(ls external_dependencies/git/cpp-benchmark/src/objs)
				expected_links_count=0
				actual_links_count=0
				for source_file in $files_in_cloned_repo; do
					link="external_dependencies/local_objs/cpp-benchmark/$source_file"
					if [ -L "$link" ]; then
						file_is_linked=$(readlink -f "$link" | grep -o "$source_file")
						if [ "$file_is_linked" != "" ]; then
							actual_links_count=`expr $actual_links_count + 1`
						fi
					fi
					expected_links_count=`expr $expected_links_count + 1`
				done
			)");

			auto expected_links_count = shell.getvar("expected_links_count").get();
			auto actual_links_count = shell.getvar("actual_links_count").get();

			assert(actual_links_count, ==, expected_links_count);
		};
	}

	test_suite("when adding dependencies with local-only dependencies") {
		setup(string, test_environment) {
			bash::shell shell;
			shell.exec(R"(
				environment_dir=$(./tests/setup_environment.sh)
				cd "$environment_dir"
				rm .assertions/dependency_manager/install.sh
				echo "#!/bin/bash" > .assertions/dependency_manager/install.sh
				./dependencies.sh add git rockerbacon/cpp-benchmark --local-only
			)");
			return shell.getvar("environment_dir").get();
		};

		teardown(environment_dir) {
			bash::shell shell;
			shell.setvar("environment_dir", environment_dir);
			shell.exec(R"(rm -rf "$environment_dir")");
		};

		test_case("should not clone local-only dependency") {
			bash::shell shell;
			shell.setvar("environment_dir", (string)*test_environment);
			shell.exec(R"(
				cd "$environment_dir"
				if [ -d "external_dependencies/git/assertions-test" ]; then
					cloned_dependency=true
				else
					cloned_dependency=false
				fi
			)");

			auto cloned_dependency = shell.getvar("cloned_dependency").get();
			assert(cloned_dependency, ==, "false");
		};

	}
}

