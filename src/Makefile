# TODO: uncomment and implement all targets

CC = gcc

GTK_FLAGS = `pkg-config --cflags gtk+-3.0`
FLAGS = -Wall -Wextra -Werror -std=c11 -pedantic -D_GNU_SOURCE

LINK_GTK = `pkg-config --libs gtk+-3.0`
LINK_LIBRARIES = -lcheck
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LINK_LIBRARIES += -lsubunit -lm
endif
#ifeq ($(UNAME_S),Darwin)
#	something for MACOS
#endif

SRC_C_FILES = lib/*.c main.c

TEST_C_FILES = unit_tests/*.c

COVERAGE_C_FILES = lib/s21_affine_transformations.c \
 				   lib/s21_free_obj_data.c \
 				   lib/s21_parse_obj_to_struct.c \
 				   lib/s21_write_data_to_file.c \
 				   unit_tests/*.c

TEST_TARGET = s21_test
TARGET = s21_viewer

REPORT_DIR = report

.PHONY: all clean install uninstall

all: clean build #test gcov_report dvi dist

install: build
	install ./$(TARGET) /usr/local/bin

uninstall:
	rm -rf /usr/local/bin/$(TARGET)

dvi: clean_dvi
	mkdir -p docs
	doxygen Doxyfile
	@mv html latex docs
	open docs/html/index.html

dist: build
	tar -czvf $(TARGET).tgz $(TARGET) graphics/*

clang:
	cp ../materials/linters/.clang-format .
	clang-format -i *.[ch]
	clang-format -i */*.[ch]
	rm -f .clang-format

build: clang $(SRC_C_FILES)
	$(CC) $(FLAGS) $(GTK_FLAGS) $(SRC_C_FILES) -o $(TARGET) $(LINK_GTK) $(LINK_LIBRARIES)

test: clang $(SRC_C_FILES)
	$(CC) $(FLAGS) $(GTK_FLAGS) $(COVERAGE_C_FILES) -o $(TEST_TARGET) $(LINK_GTK) $(LINK_LIBRARIES)
	./$(TEST_TARGET)

gcov_report: clang
	$(CC) --coverage $(FLAGS) $(GTK_FLAGS) $(COVERAGE_C_FILES) -o $(TEST_TARGET) $(LINK_GTK) $(LINK_LIBRARIES)
	./$(TEST_TARGET)
	lcov -t "$(TEST_TARGET)" -o $(TEST_TARGET).info -c -d .
	genhtml -o report $(TEST_TARGET).info
	rm *.gcda *.gcno *.info
	open report/index.html

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem .

clean_test:
	rm -f $(TEST_TARGET)

clean_report:
	rm -rf $(REPORT_DIR)

clean_lib:
	rm -f $(LIB)

clean_executables:
	rm -f $(TEST_TARGET)
	rm -f $(TARGET)

clean_gcov:
	rm -f *.gc*

clean_dvi:
	rm -rf docs/*

clean_dist:
	rm -rf $(TARGET).tgz

clean: clean_test clean_report clean_lib clean_gcov clean_dvi clean_dist clean_executables
