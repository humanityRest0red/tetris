CC := gcc
CFLAGS := -I inc -std=c11 -Wall -Werror

OS=$(shell uname)
ifeq ($(OS), Linux)
	LIB_FLAGS := -lncursesw
	CHECK_FLAGS = -lcheck -lm -lsubunit
else
	LIB_FLAGS := $(shell pkg-config --static --cflags --libs check ncurses)
	CHECK_FLAGS = -lcheck
endif

PROJECT := tetris
GUI := gui/cli/cli.c
BUILD_DIR := build
TESTS_DIR := tests

REPORT_DIR := report
DOXYGEN_DIR := doxygen
DIST_DIR := ../dist

all: gcov_report

install: tetris.a | build_dir
	$(CC) $(CFLAGS) tetris_main.c $(GUI) $(BUILD_DIR)/tetris.a -o $(BUILD_DIR)/$(PROJECT) $(LIB_FLAGS)

uninstall: 
	rm -f $(BUILD_DIR)/$(PROJECT)

dvi:
	doxygen
	open doxygen/html/index.html

dist: clean
	mkdir $(DIST_DIR)
	cp -r ../src ../dist
	cp ../README.md ../dist
	cd ../dist && tar -czvf dist.tar.gz ./src
	rm -rf ../dist/src
	
tetris.a: build_dir
	$(CC) $(CFLAGS) -c brick_game/tetris/tetris.c -o $(BUILD_DIR)/tetris.o
	ar rc $(BUILD_DIR)/tetris.a $(BUILD_DIR)/tetris.o
	ranlib $(BUILD_DIR)/tetris.a
	rm -rf $(BUILD_DIR)/tetris.o

test: uninstall install
	$(CC) $(GCC_FLAGS) ./tests/*.c ./$(BUILD_DIR)/tetris.a -o $(BUILD_DIR)/test $(CHECK_FLAGS)
	./$(BUILD_DIR)/test

gcov_report: uninstall install
	$(CC) $(GCC_FLAGS) --coverage ./brick_game/tetris/*.c ./tests/*.c ./build/tetris.a -o gcov $(CHECK_FLAGS)
	./gcov
	lcov --capture --directory . -o gcov.info
	genhtml -o report gcov.info
	rm -f gcov* *.gcda *.gcno
	open report/index.html

check: install
	cp ../materials/linters/.clang-format .
	clang-format -i ./brick_game/tetris/*.h ./brick_game/tetris/*.c ./gui/cli/*.h ./gui/cli/*.c ./tests/*.h ./tests/*.c *.c
	cppcheck --enable=all --suppress=missingIncludeSystem ./brick_game/tetris/*.h ./brick_game/tetris/*.c ./gui/cli/*.h ./gui/cli/*.c ./tests/*.h ./tests/*.c *.c
ifeq ($(OS), Linux)
	valgrind --tool=memcheck --leak-check=yes $(BUILD_DIR)/tetris
else
	leaks -atExit -- $(BUILD_DIR)/tetris
endif

out/%.o: ./brick_game/tetric/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build_dir:
	mkdir -p $(BUILD_DIR)

dist_dir:
	mkdir -p $(DIST_DIR)

debug:
	CFLAGS += -g
	make install
	gdb ./$(BUILD_DIR)/$(PROJECT)

#install-sanitized : tetris.a
#$(CC) $(CFLAGS) $(PROJECT).c $(GUI) $(BUILD_DIR)/tetris.a -o $(BUILD_DIR)/$(PROJECT) $(LIB_FLAGS) -fsanitize=address

.PHONY: install uninstall dvi dist test gcov_report clean

clean:
	rm -rf $(BUILD_DIR) $(REPORT_DIR) $(DOXYGEN_DIR) $(DIST_DIR)
	rm -rf .clang-format
	rm -rf gcov* *.gcda *.gcno