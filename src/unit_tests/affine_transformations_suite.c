#include "unit_tests.h"

Suite* get_affine_transformations_suite() {
  Suite* s = suite_create("affine_transformations_suite");

  TCase* affine_transformations_cases =
      tcase_create("affine_transformations_cases");

  suite_add_tcase(s, affine_transformations_cases);

  return s;
}