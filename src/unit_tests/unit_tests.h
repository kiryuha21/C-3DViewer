#ifndef C8_3DVIEWER_V1_0_1_UNIT_TESTS_H
#define C8_3DVIEWER_V1_0_1_UNIT_TESTS_H

#include <check.h>

#include "../s21_viewer.h"

#define SUITES_COUNT 2
#define EPS 1e-6

Suite* get_affine_transformations_suite();
Suite* get_object_reading_suite();

#endif  // C8_3DVIEWER_V1_0_1_UNIT_TESTS_H
