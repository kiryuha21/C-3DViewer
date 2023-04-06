#include "unit_tests.h"

int main() {
  setlocale(LC_NUMERIC, "ru_RU.UTF-8");
  Suite* suites[SUITES_COUNT] = {
      get_affine_transformations_suite(), get_deallocation_suite(),
      get_object_reading_suite(), get_object_writing_suite()};

  for (int i = 0; i < SUITES_COUNT; ++i) {
    SRunner* srunner = srunner_create(suites[i]);
    srunner_run_all(srunner, CK_NORMAL);
    srunner_free(srunner);
  }

  return 0;
}
