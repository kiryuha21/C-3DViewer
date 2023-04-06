#include "unit_tests.h"

START_TEST(parse_success) {
  FILE* file = fopen("test.obj", "w");
  fprintf(file,
          "v 0 0 0 #v1\n"
          "v 0 0 1 #v2\n"
          "v 0 1 1 #v3\n"
          "v 0 1 0 #v4\n"
          "f 1 2 3 4"
          "#full line comment\n");
  fclose(file);
  obj_data data;
  result_code_t result = s21_parse_obj_to_struct(&data, "test.obj");
  ck_assert_int_eq(result, SUCCESS);
  ck_assert_ptr_nonnull(data.coords);
  ck_assert_ptr_nonnull(data.facets);
  ck_assert_int_eq(data.count_of_vertexes, 4);
  ck_assert_int_eq(data.count_of_facets, 1);
  s21_free_obj_data(&data);
}

START_TEST(parse_failure) {
  FILE* file = fopen("test.obj", "w");
  fprintf(file,
          "v 0 0 0 #v1\n"
          "v 0 0 1 #v2\n"
          "v 0 1 1 #v3\n"
          "v 0 1 0 #v4\n"
          "r 1 2 3 4"
          "#full line comment\n");
  fclose(file);
  obj_data data;
  result_code_t result = s21_parse_obj_to_struct(&data, "test.obj");
  ck_assert_int_eq(result, INPUT_FORMAT_ERR);
}

START_TEST(parse_missing) {
  obj_data data;
  result_code_t result = s21_parse_obj_to_struct(&data, "not_exist.obj");
  ck_assert_int_eq(result, FILE_OPEN_ERR);
}

START_TEST(parse_wrong_input) {
  FILE* file = fopen("test.obj", "w");
  fprintf(file,
          "v 0a0 0 0 #v1\n"
          "f 1 2 3 4\n");
  fclose(file);
  obj_data data;
  result_code_t result = s21_parse_obj_to_struct(&data, "test.obj");
  ck_assert_int_eq(result, INPUT_FORMAT_ERR);
}

Suite* get_object_reading_suite() {
  Suite* s = suite_create("object_reading_suite");

  TCase* object_reading_cases = tcase_create("object_reading_cases");
  tcase_add_test(object_reading_cases, parse_success);
  tcase_add_test(object_reading_cases, parse_failure);
  tcase_add_test(object_reading_cases, parse_missing);
  tcase_add_test(object_reading_cases, parse_wrong_input);

  suite_add_tcase(s, object_reading_cases);

  return s;
}