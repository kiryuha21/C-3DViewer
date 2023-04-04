#include "s21_viewer.h"

bool starts_with(const char *str, const char *start) {
  for (; *str && *str == *start; ++str, ++start)
    ;

  return (*start) ? false : true;
}

result_code_t get_counts_from_file(obj_data *data, const char *file_name) {
  result_code_t result_code = SUCCESS;

  data->count_of_facets = 0;
  data->count_of_vertexes = 0;

  FILE *obj_file = fopen(file_name, "r");
  if (obj_file == NULL) {
    result_code = FILE_OPEN_ERR;
  }

  char *line = NULL;
  size_t cap = 0;
  while (result_code == SUCCESS && getline(&line, &cap, obj_file) != -1) {
    if (line == NULL) {
      result_code = FILE_READ_ERR;
    } else if (starts_with(line, "v ") == true) {
      ++data->count_of_vertexes;
    } else if (starts_with(line, "f ") == true) {
      ++data->count_of_facets;
    } else {
      result_code = INPUT_FORMAT_ERR;
    }
  }

  if (line != NULL) {
    free(line);
  }

  if (obj_file != NULL) {
    fclose(obj_file);
  }
  return result_code;
}

result_code_t alloc_obj_data(obj_data *data) {
  result_code_t result_code = SUCCESS;
  data->coords = (coords_t *)calloc(data->count_of_vertexes, sizeof(coords_t));

  if (data->coords == NULL) {
    result_code = ALLOC_ERR;
  }

  if (result_code == SUCCESS) {
    data->facets = (facet_t *)calloc(data->count_of_facets, sizeof(facet_t));

    if (data->facets == NULL) {
      free(data->coords);
      result_code = ALLOC_ERR;
    }
  }

  return result_code;
}

void skip_until_char(char **str, char ch) {
  while (**str != '\0' && **str != ch) {
    ++*str;
  }
}

void skip_chars(char **str, char ch) {
  while (**str == ch) {
    ++*str;
  }
}

result_code_t get_double_and_shift(char **str, double *res) {
  result_code_t result_code = SUCCESS;

  char *end_ptr = *str;
  *res = strtod(*str, &end_ptr);

  if (end_ptr == *str) {
    result_code = INPUT_FORMAT_ERR;
  } else {
    *str = end_ptr;
  }

  return result_code;
}

result_code_t get_int_and_shift(char **str, int *res) {
  result_code_t result_code = SUCCESS;

  char *end_ptr = *str;
  *res = (int)strtol(*str, &end_ptr, 10);

  if (end_ptr == *str) {
    result_code = INPUT_FORMAT_ERR;
  } else {
    *str = end_ptr;
  }

  return result_code;
}

void replace_char(char *str, char replaceable, char replacement) {
  for (; *str; ++str) {
    if (*str == replaceable) {
      *str = replacement;
    }
  }
}

result_code_t get_facet(char *line, facet_t *facet, int number) {
  result_code_t result_code = SUCCESS;
  int size = 0;

  char *str = line + 1;
  skip_chars(&str, ' ');
  while (*str) {
    skip_until_char(&str, ' ');
    skip_chars(&str, ' ');
    ++size;
  }

  facet[number].numbers_of_vertexes_in_facet = size;
  facet[number].vertexes = (int *)calloc(
      facet[number].numbers_of_vertexes_in_facet, sizeof(facet_t));
  if (facet[number].vertexes == NULL) {
    result_code = ALLOC_ERR;
  }

  if (result_code == SUCCESS) {
    int i = 0;
    str = line + 1;
    while (result_code == SUCCESS && *str) {
      result_code = get_int_and_shift(&str, &facet[number].vertexes[i]);
      facet[number].vertexes[i] -= 1;
      ++i;
      skip_until_char(&str, ' ');
      skip_chars(&str, ' ');
    }
  }

  return result_code;
}

result_code_t get_vertex(obj_data *data, char *line, int number) {
  char *val = line + 1;
  char *cp_line = val;

  result_code_t result_code =
      get_double_and_shift(&cp_line, &data->coords[number].x);

  if (result_code == SUCCESS) {
    result_code = get_double_and_shift(&cp_line, &data->coords[number].y);
  }

  if (result_code == SUCCESS) {
    result_code = get_double_and_shift(&cp_line, &data->coords[number].z);
  }

  return result_code;
}

result_code_t parse_line_to_vertexes(obj_data *data, char *line,
                                     int *count_of_vertexes) {
  result_code_t result_code;

  if (*count_of_vertexes < data->count_of_vertexes) {
    result_code = get_vertex(data, line, *count_of_vertexes);
    ++(*count_of_vertexes);
  } else {
    result_code = WRONG_COUNT_ERR;
  }

  return result_code;
}

result_code_t parse_line_to_facets(obj_data *data, char *line,
                                   int *count_of_facets) {
  result_code_t result_code;

  if (*count_of_facets < data->count_of_facets) {
    result_code = get_facet(line, data->facets, *count_of_facets);
    if (result_code != SUCCESS) {
      for (int i = 0; i < *count_of_facets; ++i) {
        free(data->facets[i].vertexes);
      }
      free(data->facets);
    }
    ++(*count_of_facets);
  } else {
    result_code = WRONG_COUNT_ERR;
  }

  return result_code;
}

result_code_t get_data_from_file(obj_data *data, const char *file_name) {
  result_code_t result_code = SUCCESS;
  int current_vertex = 0, current_facet = 0;

  FILE *obj_file = fopen(file_name, "r");
  if (obj_file == NULL) {
    result_code = FILE_OPEN_ERR;
  }

  char *line = NULL;
  size_t cap = 0;
  while (result_code == SUCCESS && getline(&line, &cap, obj_file) != -1) {
    replace_char(line, ',', '.');
    if (line == NULL) {
      result_code = FILE_READ_ERR;
    } else if (starts_with(line, "v ") == true) {
      result_code = parse_line_to_vertexes(data, line, &current_vertex);
    } else if (starts_with(line, "f ") == true) {
      result_code = parse_line_to_facets(data, line, &current_facet);
    } else {
      result_code = INPUT_FORMAT_ERR;
    }
  }

  if (line != NULL) {
    free(line);
  }

  if (obj_file != NULL) {
    fclose(obj_file);
  }

  return result_code;
}

void s21_free_obj_data(obj_data *obj_data) {
  free(obj_data->coords);

  for (int i = 0; i < obj_data->count_of_facets; ++i) {
    free(obj_data->facets[i].vertexes);
  }

  free(obj_data->facets);

  obj_data->coords = NULL;
  obj_data->facets = NULL;
}

result_code_t s21_parse_obj_to_struct(obj_data *obj_data,
                                      const char *filename) {
  result_code_t result_code = get_counts_from_file(obj_data, filename);

  if (result_code == SUCCESS) {
    result_code = alloc_obj_data(obj_data);
  }

  if (result_code == SUCCESS) {
    result_code = get_data_from_file(obj_data, filename);
    if (result_code != SUCCESS) {
      s21_free_obj_data(obj_data);
    }
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
            file_to_write, "%d%c", data->facets[i].vertexes[j] + 1,
            j == data->facets[i].numbers_of_vertexes_in_facet - 1 ? '\n' : ' ');
      }
    }
  }

  if (file_to_write != NULL) {
    fclose(file_to_write);
  }

  return result_code;
}