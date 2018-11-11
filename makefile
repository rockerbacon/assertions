CXX=g++ --std=c++14

SRC_DIR=./src
BUILD_DIR=./build
TEST_BUILD_DIR=./test_build

OBJS_DIR=./objs
OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))

UNITY_TESTS_DIR=./unity_tests
UNITY_TESTS=$(patsubst $(UNITY_TESTS_DIR)/%.cpp, $(TEST_BUILD_DIR)/%, $(wildcard $(UNITY_TESTS_DIR)/*.cpp))

HEADER_DEPS=$(SRC_DIR)/assert.h

CXXFLAGS=-I$(SRC_DIR)

clean:
	rm -f $(BUILD_DIR)/*
	rm -f $(OBJS_DIR)/*
	rm -f $(TEST_BUILD_DIR)/*

.SECONDARY: $(OBJS)

$(HEADER_DEPS):

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $^ -o $(OBJS_DIR)/$*.o $(CXXFLAGS)

$(TEST_BUILD_DIR)/%: $(UNITY_TESTS_DIR)/%.cpp $(OBJS)
	$(CXX) $^ -o $(TEST_BUILD_DIR)/$* $(CXXFLAGS)

unity_tests: $(UNITY_TESTS)
