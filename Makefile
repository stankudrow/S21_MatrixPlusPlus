ifeq ($(OS), Windows_NT)
	$(error "The Windows platform is not supported.")
endif

NAME := s21_matrix_oop

### Files and directories

LIB := $(NAME).a

SRC_DIR := ./src
HEADERS = $(shell find $(SRC_DIR) -type f -name "*.h")
HEADER_DIRS = $(shell find $(SRC_DIR) -type f -name "*.h" -exec dirname {} \; | uniq)
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.cc")
SOURCE_OBJECTS := $(SOURCES:.cc=.o)

TEST_DIR := ./tests
TEST_HEADERS := $(shell mkdir -p $(TEST_DIR); find $(TEST_DIR) -type f -name "*.h")
TEST_HEADER_DIRS = $(shell find $(TEST_DIR) -type f -name "*.h" -exec dirname {} \; | uniq)
TEST_SOURCES := $(shell find $(TEST_DIR) -type f -name "*.cc")
TEST_OBJECTS := $(TEST_SOURCES:.cc=.o)
TEST_RUNNER := $(TEST_DIR)/test_runner.out

ALL_HEADERS := $(HEADERS) $(TEST_HEADERS)
ALL_SOURCES := $(SOURCES) $(TEST_SOURCES)
ALL_FILES := $(ALL_HEADERS) $(ALL_SOURCES)

### Commands and options

# -fsanitize=address
CC := g++
CFLAGS := -Wall -Werror -Wextra -lstdc++ --std=c++17
DFLAGS := -g
GFLAGS := -lgtest -lgtest_main -lgmock -lpthread
GTRUN_FLAGS := --gtest_break_on_failure --gtest_shuffle

CFORMAT := clang-format
FORMAT_GSTYLE := $(CFORMAT) -style=google

SRC_HEADERS_INCS := $(foreach HDIR, $(HEADER_DIRS), -I $(HDIR))
TEST_HEADERS_INCS := $(foreach THDIR, $(TEST_HEADER_DIRS), -I $(THDIR))
INCS := $(TEST_HEADERS_INCS) $(SRC_HEADERS_INCS)

### Coverage

COV_DIR := ./gcov_report
COV_OUT := $(NAME).report
COV_INFO := $(NAME).info
REPORT_DIR := $(COV_DIR)/report

### Checkers

LEAKS := leaks
LEAKS_OPTS := --atExit --

### Targets

.PHONY: all clean re format style test test-leaks cov clean-cov

# all: $(LIB)

# $(LIB): $(SOURCE_OBJECTS)
# 	ar -rcs $(LIB) $(SOURCE_OBJECTS)
# 	rm -rf $(SOURCE_OBJECTS)

# %.o: %.cc $(HEADERS)
# 	$(CC) $(CFLAGS) $(SRC_HEADERS_INCS) -c $< -o $@

clean:
	rm -rf $(shell find -E . -regex '.*\.(d|o|out|dSYM)')
	@make clean-cov
	rm -f $(LIB)

re: clean all

format:
	$(FORMAT_GSTYLE) -i $(ALL_FILES)

style:
	$(FORMAT_GSTYLE) -n $(ALL_FILES)

test:
	$(CC) $(CFLAGS) $(GFLAGS) $(TEST_SOURCES) $(INCS) -o $(TEST_RUNNER)
	$(TEST_RUNNER) $(GTRUN_FLAGS)

test-leaks: test
	$(LEAKS) $(LEAKS_OPTS) $(TEST_RUNNER) $(GTRUN_FLAGS)

# https://ps-group.github.io/cxx/coverage_gcc
cov: clean-cov $(LIB)
	@mkdir -p $(COV_DIR)
	$(CC) $(CFLAGS) $(GFLAGS) --coverage $(INCS) $(ALL_SOURCES) -o $(COV_OUT)
	./$(COV_OUT)
	lcov -b . -c -d . -t $(COV_NAME) -c -o $(COV_INFO)
	lcov -r $(COV_INFO) "/usr*" -o $(COV_INFO)
	genhtml -o $(REPORT_DIR) $(COV_INFO)
	@mv *.gcda *.gcno *.info *.report $(COV_DIR)
	open $(REPORT_DIR)/index.html

clean-cov:
	rm -rf $(COV_DIR)
