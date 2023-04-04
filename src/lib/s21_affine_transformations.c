#include "s21_viewer.h"

void s21_move(obj_data *data, double x, double y, double z) {
  for (int i = 0; i < data->count_of_vertexes; i++) {
    data->coords[i].x += x;
    data->coords[i].y += y;
    data->coords[i].z += z;
  }
}

result_code_t s21_scale(obj_data *data, double x, double y, double z) {
  result_code_t result_code = SUCCESS;
  if (x != 0 && y != 0 && z != 0) {
    for (int i = 0; i < data->count_of_vertexes; i++) {
      data->coords[i].x *= x;
      data->coords[i].y *= y;
      data->coords[i].z *= z;
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

void s21_rotate(obj_data *data, double x_angle, double y_angle,
                double z_angle) {
  for (int i = 0; i < data->count_of_vertexes; i++) {
    rotate_on_x(&data->coords[i], x_angle);
    rotate_on_y(&data->coords[i], y_angle);
    rotate_on_z(&data->coords[i], z_angle);
  }
}
