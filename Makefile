# https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS), Windows_NT)
	$(error "The Windows platform is not supported.")
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME), Linux)
		LEAKS := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
		OPEN := xdg-open
	endif
	ifeq ($(UNAME), Darwin)
		LEAKS := leaks --atExit --
		OPEN := open
	endif
endif

NAME := s21_matrix_oop

### Files and directories

SRC_DIR := ./src
HEADERS := $(shell mkdir -p $(SRC_DIR); find $(SRC_DIR) -type f -name "*.h")
HEADER_DIRS = $(shell find $(SRC_DIR) -type f -name "*.h" -exec dirname {} \; | uniq)

TEST_DIR := ./tests
TEST_HEADERS := $(shell mkdir -p $(TEST_DIR); find $(TEST_DIR) -type f -name "*.h")
TEST_HEADER_DIRS = $(shell find $(TEST_DIR) -type f -name "*.h" -exec dirname {} \; | uniq)
TEST_SOURCES := $(shell find $(TEST_DIR) -type f -name "*.cc")
TEST_OBJECTS := $(TEST_SOURCES:.cc=.o)
TEST_RUNNER := $(TEST_DIR)/test_runner.out

ALL_HEADERS := $(HEADERS) $(TEST_HEADERS)
ALL_SOURCES := $(TEST_SOURCES)
ALL_FILES := $(ALL_HEADERS) $(ALL_SOURCES)

### Commands and options

# -fsanitize=address
CXX := g++
CXXFLAGS := -Wall -Werror -Wextra --std=c++17
DBG_FLAGS := -g
GTEST_FLAGS := -lgtest -lgtest_main -lpthread
GTEST_RUN_FLAGS := --gtest_break_on_failure --gtest_shuffle

CFORMAT := clang-format
CFORMAT_GSTYLE := $(CFORMAT) -style=google

SRC_HEADERS_INCS := $(foreach HDIR, $(HEADER_DIRS), -I $(HDIR))
TEST_HEADERS_INCS := $(foreach THDIR, $(TEST_HEADER_DIRS), -I $(THDIR))
INCS := $(TEST_HEADERS_INCS) $(SRC_HEADERS_INCS)

### Coverage

COV_DIR := ./gcov_report
COV_OUT := $(NAME).report
COV_INFO := $(NAME).info
REPORT_DIR := $(COV_DIR)/report

### Targets

.PHONY: all clean re format style test test-leaks test-rebuild test-re cov clean-cov

all:
	make style
	make test-leaks

clean:
	rm -rf $(shell find -E . -regex '.*\.(d|o|out|dSYM)')
	@make clean-cov

re: clean all

format:
	$(CFORMAT_GSTYLE) -i $(ALL_FILES)

style:
	$(CFORMAT_GSTYLE) -n $(ALL_FILES)

$(TEST_RUNNER):
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) $(INCS) -o $(TEST_RUNNER) $(GTEST_FLAGS)

test: $(TEST_RUNNER)
	$(TEST_RUNNER) $(GTEST_RUN_FLAGS)

test-leaks: $(TEST_RUNNER)
	$(LEAKS) $(TEST_RUNNER) $(GTEST_RUN_FLAGS)

test-rebuild:
	rm -f $(TEST_RUNNER)
	@make $(TEST_RUNNER)

test-re: test-rebuild
	@make test

# https://ps-group.github.io/cxx/coverage_gcc
cov: clean-cov
	@mkdir -p $(COV_DIR)
	$(CXX) $(CXXFLAGS) --coverage $(INCS) $(ALL_SOURCES) -o $(COV_OUT) $(GTEST_FLAGS)
	./$(COV_OUT)
	lcov -b . -c -d . -t $(COV_NAME) -c -o $(COV_INFO)
	lcov -r $(COV_INFO) "/usr*" -o $(COV_INFO)
	genhtml -o $(REPORT_DIR) $(COV_INFO)
	@mv *.gcda *.gcno *.info *.report $(COV_DIR)
	$(OPEN) $(REPORT_DIR)/index.html

clean-cov:
	rm -rf $(COV_DIR)
