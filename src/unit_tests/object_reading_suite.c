#include "unit_tests.h"

Suite* get_object_reading_suite() {
  Suite* s = suite_create("object_reading_suite");

  TCase* object_reading_cases = tcase_create("object_reading_cases");

  suite_add_tcase(s, object_reading_cases);

  return s;
}