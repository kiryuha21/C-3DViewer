#include "unit_tests.h"

result_code_t compare_files(FILE* fPtr1, FILE* fPtr2) {
  char ch1, ch2;

  result_code_t res = SUCCESS;

  do {
    ch1 = fgetc(fPtr1);
    ch2 = fgetc(fPtr2);

    if (ch1 != ch2) {
      res = INPUT_FORMAT_ERR;
    }

  } while (ch1 != EOF && ch2 != EOF && res == SUCCESS);

  if (!(ch1 == EOF && ch2 == EOF)) {
    res = INPUT_FORMAT_ERR;
  }

  return res;
}

START_TEST(test1) {
  obj_data data;

  result_code_t result_code =
      s21_parse_obj_to_struct(&data, "obj_files/diamond.obj");
  ck_assert_int_eq(result_code, SUCCESS);

  coords_t move = {1, 1, 1};
  coords_t scale = {1, 2, 3};
  coords_t rotate = {30, 60, 90};
  s21_move(&data, &move);
  s21_scale(&data, &scale);
  s21_rotate(&data, &rotate);

  result_code = s21_write_coords_to_file(&data, "unit_tests/diamond.cobj");
  ck_assert_int_eq(result_code, SUCCESS);

  FILE *file1 = fopen("unit_tests/diamond.cobj", "r"),
       *file2 = fopen("unit_tests/results/diamond.cobj", "r");
  if (file1 != NULL && file2 != NULL) {
    ck_assert_int_eq(compare_files(file1, file2), SUCCESS);
  }
  fclose(file1);
  fclose(file2);

  result_code = s21_write_obj_to_file(&data, "unit_tests/diamond.robj");
  ck_assert_int_eq(result_code, SUCCESS);

  obj_data answer;
  result_code =
      s21_parse_obj_to_struct(&answer, "unit_tests/results/diamond.robj");
  ck_assert_int_eq(result_code, SUCCESS);

  ck_assert_int_eq(answer.count_of_vertexes, data.count_of_vertexes);
  ck_assert_int_eq(answer.count_of_facets, data.count_of_facets);
  for (int i = 0; i < answer.count_of_vertexes; ++i) {
    ck_assert_double_eq_tol(data.coords[i].x, answer.coords[i].x, EPS);
    ck_assert_double_eq_tol(data.coords[i].y, answer.coords[i].y, EPS);
    ck_assert_double_eq_tol(data.coords[i].z, answer.coords[i].z, EPS);
  }

  for (int i = 0; i < answer.count_of_facets; ++i) {
    ck_assert_int_eq(answer.facets[i].numbers_of_vertexes_in_facet,
                     data.facets[i].numbers_of_vertexes_in_facet);
    for (int j = 0; j < answer.facets[i].numbers_of_vertexes_in_facet; ++j) {
      ck_assert_int_eq(answer.facets[i].vertexes[j],
                       data.facets[i].vertexes[j]);
    }
  }

  s21_free_obj_data(&answer);
  s21_free_obj_data(&data);
}
END_TEST

Suite* get_affine_transformations_suite() {
  Suite* s = suite_create("affine_transformations_suite");

  TCase* affine_transformations_cases =
      tcase_create("affine_transformations_cases");

  suite_add_tcase(s, affine_transformations_cases);

  tcase_add_test(affine_transformations_cases, test1);

  return s;
}