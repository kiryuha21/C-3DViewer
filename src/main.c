#include "s21_viewer.h"

int main(int argc, char* argv[]) {
  gtk_init(&argc, &argv);
  GError* error = NULL;

  GtkBuilder* builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, MAIN_UI_FILE, &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return ERR;
  }

  GtkCssProvider* provider = gtk_css_provider_new();
  if (gtk_css_provider_load_from_path(provider, STYLES_FILE, &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return ERR;
  }
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

  GObject* main_window = gtk_builder_get_object(builder, "main_window");
  g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  obj_data* object = calloc(1, sizeof(obj_data));
  GPtrArray* data = g_ptr_array_new();
  g_ptr_array_add(data, builder);
  g_ptr_array_add(data, object);

  GObject* render_button = gtk_builder_get_object(builder, "render_button");
  g_signal_connect(render_button, "clicked", G_CALLBACK(render_with_deltas),
                   data);

  gtk_main();

  s21_free_obj_data(object);
  free(object);
  g_ptr_array_free(data, false);

  return 0;
}
