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
  double z = coords->z, y = coords->y;
  coords->y = y * cos(angle) + z * sin(angle);
  coords->z = -y * sin(angle) + z * cos(angle);
}

void rotate_on_y(coords_t *coords, double angle) {
  angle = to_degrees(angle);
  double z = coords->z, x = coords->x;
  coords->x = x * cos(angle) + z * sin(angle);
  coords->z = -x * sin(angle) + z * cos(angle);
}

void rotate_on_z(coords_t *coords, double angle) {
  angle = to_degrees(angle);
  double x = coords->x, y = coords->y;
  coords->x = x * cos(angle) - y * sin(angle);
  coords->y = x * sin(angle) + y * cos(angle);
}

void s21_rotate(obj_data *data, const coords_t *angles) {
  for (int i = 0; i < data->count_of_vertexes; i++) {
    rotate_on_x(&data->coords[i], angles->x);
    rotate_on_y(&data->coords[i], angles->y);
    rotate_on_z(&data->coords[i], angles->z);
  }
}
