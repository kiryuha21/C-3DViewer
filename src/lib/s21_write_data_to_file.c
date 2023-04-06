#include "../s21_viewer.h"

result_code_t s21_write_coords_to_file(const obj_data *data,
                                       const char *filename) {
  result_code_t result_code = SUCCESS;

  FILE *file_to_write = fopen(filename, "w");
  if (file_to_write == NULL) {
    result_code = FILE_OPEN_ERR;
  }

  if (result_code == SUCCESS) {
    for (int i = 0; i < data->count_of_facets; ++i) {
      for (int j = 0; j < data->facets[i].numbers_of_vertexes_in_facet; ++j) {
        fprintf(file_to_write, "%lf %lf %lf\n",
                data->coords[data->facets[i].vertexes[j]].x,
                data->coords[data->facets[i].vertexes[j]].y,
                data->coords[data->facets[i].vertexes[j]].z);
      }
      fprintf(file_to_write, "\n");
    }
  }

  s21_safe_fclose(&file_to_write);

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
            file_to_write, "%d%c", data->facets[i].vertexes[j] + 1,
            j == data->facets[i].numbers_of_vertexes_in_facet - 1 ? '\n' : ' ');
      }
    }
  }

  s21_safe_fclose(&file_to_write);

  return result_code;
}