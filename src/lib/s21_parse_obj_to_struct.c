#include "../s21_viewer.h"

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

void replace_char(char *str, char replaceable, char replacement) {
  for (; *str; ++str) {
    if (*str == replaceable) {
      *str = replacement;
    }
  }
}

bool starts_with(char *str, const char *start) {
  skip_chars(&str, ' ');
  for (; *str && *str == *start; ++str, ++start)
    ;

  return (*start) ? false : true;
}

bool line_ended(char *line) {
  skip_chars(&line, ' ');
  return (*line == '\0' || *line == '\n' || *line == '#') ? true : false;
}

bool valid_obj(const obj_data *data) {
  bool result = true;

  for (int i = 0; i < data->count_of_facets && result == true; ++i) {
    for (int j = 0;
         j < data->facets[i].numbers_of_vertexes_in_facet && result == true;
         ++j) {
      if (data->facets[i].vertexes[j] < 0 ||
          data->facets[i].vertexes[j] > data->count_of_vertexes - 1) {
        result = false;
      }
    }
  }

  return result;
}

result_code_t read_double_and_shift(char **str, double *res) {
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

result_code_t read_int_and_shift(char **str, int *res) {
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

result_code_t read_vertex_from_line(obj_data *data, char *line, int number) {
  skip_chars(&line, ' ');
  ++line;

  result_code_t result_code =
      read_double_and_shift(&line, &data->coords[number].x);

  if (result_code == SUCCESS) {
    result_code = read_double_and_shift(&line, &data->coords[number].y);
  }

  if (result_code == SUCCESS) {
    result_code = read_double_and_shift(&line, &data->coords[number].z);
  }

  if (line_ended(line) == false) {
    result_code = INPUT_FORMAT_ERR;
  }

  return result_code;
}

result_code_t parse_line_to_vertex(obj_data *data, char *line,
                                   int *count_of_vertexes) {
  result_code_t result_code;

  if (*count_of_vertexes < data->count_of_vertexes) {
    result_code = read_vertex_from_line(data, line, *count_of_vertexes);
    ++(*count_of_vertexes);
  } else {
    result_code = WRONG_COUNT_ERR;
  }

  return result_code;
}

result_code_t read_facet_from_line(char *line, facet_t *facet, int number) {
  result_code_t result_code = SUCCESS;
  skip_chars(&line, ' ');
  ++line;

  char *start = line;

  int size = 0;
  while (line_ended(line) == false) {
    skip_chars(&line, ' ');
    skip_until_char(&line, ' ');
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
    line = start;
    while (result_code == SUCCESS && line_ended(line) == false) {
      skip_chars(&line, ' ');
      result_code = read_int_and_shift(&line, &facet[number].vertexes[i]);
      facet[number].vertexes[i] -= 1;
      ++i;
    }
  }

  return result_code;
}

result_code_t parse_line_to_facet(obj_data *data, char *line,
                                  int *count_of_facets) {
  result_code_t result_code;

  if (*count_of_facets < data->count_of_facets) {
    result_code = read_facet_from_line(line, data->facets, *count_of_facets);
    if (result_code != SUCCESS) {
      for (int i = 0; i < *count_of_facets; ++i) {
        free(data->facets[i].vertexes);
      }
      data->count_of_facets = 0;
    }
    ++(*count_of_facets);
  } else {
    result_code = WRONG_COUNT_ERR;
  }

  return result_code;
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
    } else if (starts_with(line, "#") == false && line_ended(line) == false) {
      result_code = INPUT_FORMAT_ERR;
    }
  }

  free(line);
  s21_safe_fclose(&obj_file);

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
      data->count_of_vertexes = 0;
      result_code = ALLOC_ERR;
    }
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
      result_code = parse_line_to_vertex(data, line, &current_vertex);
    } else if (starts_with(line, "f ") == true) {
      result_code = parse_line_to_facet(data, line, &current_facet);
    } else if (starts_with(line, "#") == false && line_ended(line) == false) {
      result_code = INPUT_FORMAT_ERR;
    }
  }

  s21_safe_free(&line);
  s21_safe_fclose(&obj_file);

  return result_code;
}

result_code_t s21_parse_obj_to_struct(obj_data *obj_data,
                                      const char *filename) {
  result_code_t result_code = get_counts_from_file(obj_data, filename);

  if (result_code == SUCCESS) {
    result_code = alloc_obj_data(obj_data);
  }

  if (result_code == SUCCESS) {
    result_code = get_data_from_file(obj_data, filename);
    if (result_code != SUCCESS || valid_obj(obj_data) == false) {
      s21_free_obj_data(obj_data);
    }
  }

  return result_code;
}