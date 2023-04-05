#include "../s21_viewer.h"

bool is_null_or_empty(const gchar* string) {
  bool return_value = true;

  if (string != NULL && strcmp(string, "") != 0) {
    size_t len = strlen(string);
    for (size_t i = 0; i < len && return_value == true; ++i) {
      if (isspace(string[i]) == false) {
        return_value = false;
      }
    }
  }

  return return_value;
}

bool convert_to_double(const gchar* string, double* val) {
  bool return_val = true;
  char* endptr = NULL;
  *val = strtod(string, &endptr);
  if (strcmp(endptr, "\0") != 0) {
    return_val = false;
  }
  return return_val;
}

void render_with_deltas(GtkWidget* widget, gpointer user_data) {
  UNUSED(widget);

  GPtrArray* delta_data = collect_delta_data(user_data);
  if (delta_data->len != 0) {
//    obj_data data;
//    result_code_t result_code =
//        s21_parse_obj_to_struct(&data, "obj_files/test1.obj");
//    s21_write_coords_to_file(&data, "obj_files/test1.robj");
//    if (result_code == SUCCESS) {
//      s21_free_obj_data(&data);
//    }
  }
  g_ptr_array_free(delta_data, false);
}

GPtrArray* collect_delta_data(GtkBuilder* builder) {
  GPtrArray* delta_data = g_ptr_array_new();

  double scale_delta = 0;
  coords_t coords = {0, 0, 0}, angles = {0, 0, 0};

  GObject* scale_entry = gtk_builder_get_object(builder, "scale_entry");
  GObject* x_coord = gtk_builder_get_object(builder, "x_coord_delta_entry");
  GObject* y_coord = gtk_builder_get_object(builder, "y_coord_delta_entry");
  GObject* z_coord = gtk_builder_get_object(builder, "z_coord_delta_entry");
  GObject* x_angle = gtk_builder_get_object(builder, "x_angle_delta_entry");
  GObject* y_angle = gtk_builder_get_object(builder, "y_angle_delta_entry");
  GObject* z_angle = gtk_builder_get_object(builder, "z_angle_delta_entry");

  const gchar* scale_text = gtk_entry_get_text(GTK_ENTRY(scale_entry));
  const gchar* x_coord_text = gtk_entry_get_text(GTK_ENTRY(x_coord));
  const gchar* y_coord_text = gtk_entry_get_text(GTK_ENTRY(y_coord));
  const gchar* z_coord_text = gtk_entry_get_text(GTK_ENTRY(z_coord));
  const gchar* x_angle_text = gtk_entry_get_text(GTK_ENTRY(x_angle));
  const gchar* y_angle_text = gtk_entry_get_text(GTK_ENTRY(y_angle));
  const gchar* z_angle_text = gtk_entry_get_text(GTK_ENTRY(z_angle));

  if (convert_to_double(scale_text, &scale_delta) == false ||
      convert_to_double(x_coord_text, &coords.x) == false ||
      convert_to_double(y_coord_text, &coords.y) == false ||
      convert_to_double(z_coord_text, &coords.z) == false ||
      convert_to_double(x_angle_text, &angles.x) == false ||
      convert_to_double(y_angle_text, &angles.y) == false ||
      convert_to_double(z_angle_text, &angles.z) == false) {
    GObject* label = gtk_builder_get_object(builder, "viewer_label");
    gtk_label_set_label(GTK_LABEL(label), WRONG_INPUT_MSG);
  } else {
    g_ptr_array_add(delta_data, &scale_delta);
    g_ptr_array_add(delta_data, &coords);
    g_ptr_array_add(delta_data, &angles);
  }

  return delta_data;
}