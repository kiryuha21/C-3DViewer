#include "s21_viewer.h"

void s21_free_obj_data(obj_data *obj_data) {
  free(obj_data->coords);

  for (int i = 0; i < obj_data->count_of_facets; ++i) {
    free(obj_data->facets[i].vertexes);
  }

  free(obj_data->facets);

  obj_data->coords = NULL;
  obj_data->facets = NULL;
  obj_data->count_of_facets = 0;
  obj_data->count_of_vertexes = 0;
}

void s21_safe_fclose(FILE **file) {
  if (*file != NULL) {
    fclose(*file);
  }
}

void s21_safe_free(char **str) {
  if (*str != NULL) {
    free(*str);
  }
}