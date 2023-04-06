#include "unit_tests.h"

Suite* get_object_writing_suite() {
  Suite* s = suite_create("object_writing_suite");

  TCase* object_writing_cases = tcase_create("object_writing_cases");

  suite_add_tcase(s, object_writing_cases);

  return s;
}