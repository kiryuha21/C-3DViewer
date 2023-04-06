#include "unit_tests.h"

Suite* get_deallocation_suite() {
  Suite* s = suite_create("deallocation_suite");

  TCase* deallocation_cases = tcase_create("deallocation_cases");

  suite_add_tcase(s, deallocation_cases);

  return s;
}