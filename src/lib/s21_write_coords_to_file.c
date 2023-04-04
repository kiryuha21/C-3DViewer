#include "s21_viewer.h"

result_code_t s21_write_coords_to_file(const obj_data *data,
                                       const char *filename) {
  result_code_t result_code = SUCCESS;

  FILE *file_to_write = fopen(filename, "w");
  if (file_to_write == NULL) {
    result_code = FILE_OPEN_ERR;
  }

  if (result_code == SUCCESS) {
    for (int i = 0; i < data->count_of_vertexes; ++i) {
      fprintf(file_to_write, "%lf %lf %lf\n", data->coords[i].x,
              data->coords[i].y, data->coords[i].z);
    }
  }

  if (file_to_write != NULL) {
    fclose(file_to_write);
  }

  return result_code;
}

result_code_t s21_write_obj_to_file(const obj_data *data,
                                    const char *filename) {
  result_code_t result_code = SUCCESS;

  FILE *file_to_write = fopen(filename, "w");
  if (file_to_write == NULL) {
    result_code = FILE_OPEN_ERR;
  }

  if (result_code == SUCCESS) {
    for (int i = 0; i < data->count_of_vertexes; ++i) {
      fprintf(file_to_write, "v %lf %lf %lf\n", data->coords[i].x,
              data->coords[i].y, data->coords[i].z);
    }

    for (int i = 0; i < data->count_of_facets; ++i) {
      fprintf(file_to_write, "f ");
      for (int j = 0; j < data->facets[i].numbers_of_vertexes_in_facet; ++j) {
        fprintf(
            file_to_write, "v%d%c", data->facets[i].vertexes[j] + 1,
            j == data->facets[i].numbers_of_vertexes_in_facet - 1 ? '\n' : ' ');
      }
    }
  }

  if (file_to_write != NULL) {
    fclose(file_to_write);
  }

  return result_code;
}