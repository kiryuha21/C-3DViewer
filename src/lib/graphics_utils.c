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

void render_with_deltas(GtkWidget* widget, gpointer builder) {
  UNUSED(widget);

  GtkWidget* image =
      GTK_WIDGET(gtk_builder_get_object(builder, "visualization_image"));
  GtkFileChooser* file_chooser =
      GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "file_selector"));
  GPtrArray* delta_data = collect_delta_data(builder);

  if (delta_data->len != 0) {
    gchar* filename = gtk_file_chooser_get_filename(file_chooser);
    if (is_null_or_empty(filename)) {
      GObject* label = gtk_builder_get_object(builder, "viewer_label");
      gtk_label_set_label(GTK_LABEL(label), MISSING_FILE_MSG);
    } else {
      obj_data data;
      result_code_t result_code = s21_parse_obj_to_struct(&data, filename);
      if (result_code == SUCCESS) {
        coords_t* scales_delta = delta_data->pdata[0];
        s21_scale(&data, scales_delta);  // TODO: check return value?

        coords_t* coords_delta = delta_data->pdata[1];
        s21_move(&data, coords_delta);

        coords_t* angles_delta = delta_data->pdata[2];
        s21_rotate(&data, angles_delta);

        s21_write_coords_to_file(&data, POINTS_FILE);
        gnuplot_call_wrapper(image);
        s21_free_obj_data(&data);
      }
    }
    g_free(filename);
  }
  g_ptr_array_free(delta_data, true);
}

GPtrArray* collect_delta_data(GtkBuilder* builder) {
  GPtrArray* delta_data = g_ptr_array_new();

  coords_t* coords = calloc(1, sizeof(coords_t));
  coords_t* angles = calloc(1, sizeof(coords_t));
  coords_t* scales = calloc(1, sizeof(coords_t));

  GObject* x_scale = gtk_builder_get_object(builder, "x_scale_delta_entry");
  GObject* y_scale = gtk_builder_get_object(builder, "y_scale_delta_entry");
  GObject* z_scale = gtk_builder_get_object(builder, "z_scale_delta_entry");
  GObject* x_coord = gtk_builder_get_object(builder, "x_coord_delta_entry");
  GObject* y_coord = gtk_builder_get_object(builder, "y_coord_delta_entry");
  GObject* z_coord = gtk_builder_get_object(builder, "z_coord_delta_entry");
  GObject* x_angle = gtk_builder_get_object(builder, "x_angle_delta_entry");
  GObject* y_angle = gtk_builder_get_object(builder, "y_angle_delta_entry");
  GObject* z_angle = gtk_builder_get_object(builder, "z_angle_delta_entry");

  const gchar* x_scale_text = gtk_entry_get_text(GTK_ENTRY(x_scale));
  const gchar* y_scale_text = gtk_entry_get_text(GTK_ENTRY(y_scale));
  const gchar* z_scale_text = gtk_entry_get_text(GTK_ENTRY(z_scale));
  const gchar* x_coord_text = gtk_entry_get_text(GTK_ENTRY(x_coord));
  const gchar* y_coord_text = gtk_entry_get_text(GTK_ENTRY(y_coord));
  const gchar* z_coord_text = gtk_entry_get_text(GTK_ENTRY(z_coord));
  const gchar* x_angle_text = gtk_entry_get_text(GTK_ENTRY(x_angle));
  const gchar* y_angle_text = gtk_entry_get_text(GTK_ENTRY(y_angle));
  const gchar* z_angle_text = gtk_entry_get_text(GTK_ENTRY(z_angle));

  if (convert_to_double(x_scale_text, &scales->x) == false ||
      convert_to_double(y_scale_text, &scales->y) == false ||
      convert_to_double(z_scale_text, &scales->z) == false ||
      convert_to_double(x_coord_text, &coords->x) == false ||
      convert_to_double(y_coord_text, &coords->y) == false ||
      convert_to_double(z_coord_text, &coords->z) == false ||
      convert_to_double(x_angle_text, &angles->x) == false ||
      convert_to_double(y_angle_text, &angles->y) == false ||
      convert_to_double(z_angle_text, &angles->z) == false) {
    GObject* label = gtk_builder_get_object(builder, "viewer_label");
    gtk_label_set_label(GTK_LABEL(label), WRONG_INPUT_MSG);
  } else {
    g_ptr_array_add(delta_data, scales);
    g_ptr_array_add(delta_data, coords);
    g_ptr_array_add(delta_data, angles);
  }

  return delta_data;
}
