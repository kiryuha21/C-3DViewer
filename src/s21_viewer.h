#ifndef C8_3DVIEWER_V1_0_1_S21_VIEWER_H
#define C8_3DVIEWER_V1_0_1_S21_VIEWER_H

#define MAIN_UI_FILE "graphics/main.ui"
#define MAIN_STYLES_FILE "graphics/styles.css"

#include <gtk/gtk.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  SUCCESS,
  ERR,
  FILE_OPEN_ERR,
  FILE_READ_ERR,
  ALLOC_ERR,
  WRONG_COUNT_ERR,
  INPUT_FORMAT_ERR,
  ZERO_SCALING_ERR
} result_code_t;

typedef struct coords {
  double x;
  double y;
  double z;
} coords_t;

typedef struct facets {
  int *vertexes;
  int numbers_of_vertexes_in_facet;
} facet_t;

typedef struct obj_data {
  int count_of_vertexes;
  coords_t *coords;
  int count_of_facets;
  facet_t *facets;
} obj_data;

// s21_parse_obj_to_struct.c
result_code_t s21_parse_obj_to_struct(obj_data *obj_data, const char *filename);

// s21_write_data_to_file.c
result_code_t s21_write_obj_to_file(const obj_data *data, const char *filename);
result_code_t s21_write_coords_to_file(const obj_data *data,
                                       const char *filename);

// s21_free_obj_data.c
void s21_free_obj_data(obj_data *obj_data);
void s21_safe_fclose(FILE **file);
void s21_safe_free(char **str);

// s21_affine_transformations.c
void s21_move(obj_data *data, double x, double y, double z);
result_code_t s21_scale(obj_data *data, double x, double y, double z);
void s21_rotate(obj_data *data, double x_angle, double y_angle, double z_angle);

#endif  // C8_3DVIEWER_V1_0_1_S21_VIEWER_H
