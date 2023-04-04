#ifndef C8_3DVIEWER_V1_0_1_S21_VIEWER_H
#define C8_3DVIEWER_V1_0_1_S21_VIEWER_H

#define MAIN_UI_FILE "graphics/main.ui"
#define MAIN_STYLES_FILE "graphics/styles.css"

#include <gtk/gtk.h>
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
  INPUT_FORMAT_ERR
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

result_code_t s21_parse_obj_to_struct(obj_data *obj_data, const char *filename);
result_code_t s21_write_obj_to_file(const obj_data *data, const char *filename);
void s21_free_obj_data(obj_data *obj_data);

#endif  // C8_3DVIEWER_V1_0_1_S21_VIEWER_H
