#ifndef C8_3DVIEWER_V1_0_1_S21_VIEWER_H
#define C8_3DVIEWER_V1_0_1_S21_VIEWER_H

#include <ctype.h>
#include <gtk/gtk.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAIN_UI_FILE "graphics/main.ui"
#define STYLES_FILE "graphics/styles.css"
#define IMAGE_OUTPUT "graphics/rendered_image.png"
#define POINTS_FILE "obj_files/points.txt"

#define INVALID_FILE_MSG "Invalid object file!"
#define MISSING_FILE_MSG "Missing object file!"
#define WRONG_INPUT_MSG "Wrong input data!"
#define GNUPLOT_FORMAT                    \
  "set terminal pngcairo size %d,%d\n"    \
  "set output '%s'\n"                     \
  "set decimalsign locale\n"              \
  "set xyplane at 0\n"                    \
  "set view equal xyz\n"                  \
  "unset border\n"                        \
  "set xlabel \"X-Axis\"\n"               \
  "set ylabel \"Y-Axis\"\n"               \
  "set zlabel \"Z-Axis\"\n"               \
  "set pm3d depth\n"                      \
  "set pm3d border lc \"black\" lw 1.5\n" \
  "splot '%s' notitle with polygons fs transparent solid 0.8 fc \"gray75\""

#define UNUSED(x) (void)(x)

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
void s21_move(obj_data *data, const coords_t *move_on);
result_code_t s21_scale(obj_data *data, const coords_t *scale_on);
void s21_rotate(obj_data *data, const coords_t *angles);

bool is_null_or_empty(const gchar *string);
GPtrArray *collect_delta_data(GtkBuilder *builder);
void render_with_deltas(GtkWidget *widget, gpointer builder);
void call_gnuplot(GtkWidget *plot_image);

#endif  // C8_3DVIEWER_V1_0_1_S21_VIEWER_H
