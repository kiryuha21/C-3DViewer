#include "../s21_viewer.h"

void s21_move(obj_data *data, const coords_t *move_on) {
  for (int i = 0; i < data->count_of_vertexes; i++) {
    data->coords[i].x += move_on->x;
    data->coords[i].y += move_on->y;
    data->coords[i].z += move_on->z;
  }
}

result_code_t s21_scale(obj_data *data, const coords_t *scale_on) {
  result_code_t result_code = SUCCESS;
  if (scale_on->x != 0 && scale_on->y != 0 && scale_on->z != 0) {
    for (int i = 0; i < data->count_of_vertexes; i++) {
      data->coords[i].x *= scale_on->x;
      data->coords[i].y *= scale_on->y;
      data->coords[i].z *= scale_on->z;
    }
  } else {
    result_code = ZERO_SCALING_ERR;
  }
  return result_code;
}

double to_degrees(double val) { return val * M_PI / 180; }

void rotate_on_x(coords_t *coords, double angle) {
  angle = to_degrees(angle);
  double y_st = coords->y;
  coords->y = coords->y * cos(angle) + coords->z * sin(angle);
  coords->z = -y_st * sin(angle) + coords->z * cos(angle);
}

void rotate_on_y(coords_t *coords, double angle) {
  angle = to_degrees(angle);
  double x_st = coords->x;
  coords->x = coords->x * cos(angle) + coords->z * sin(angle);
  coords->z = -x_st * sin(angle) + coords->z * cos(angle);
}

void rotate_on_z(coords_t *coords, double angle) {
  angle = to_degrees(angle);
  double x_st = coords->x;
  coords->x = coords->x * cos(angle) - coords->y * sin(angle);
  coords->y = -x_st * sin(angle) + coords->y * cos(angle);
}

void s21_rotate(obj_data *data, const coords_t *angles) {
  for (int i = 0; i < data->count_of_vertexes; i++) {
    rotate_on_x(&data->coords[i], angles->x);
    rotate_on_y(&data->coords[i], angles->y);
    rotate_on_z(&data->coords[i], angles->z);
  }
}
