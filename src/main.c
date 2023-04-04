#include "s21_viewer.h"

int main(int argc, char* argv[]) {
//  obj_data data;
//  result_code_t result_code =
//      s21_parse_obj_to_struct(&data, "obj_files/test1.obj");
//  s21_write_obj_to_file(&data, "obj_files/test1.robj");
//  if (result_code == SUCCESS) {
//    s21_free_obj_data(&data);
//  }

  gtk_init(&argc, &argv);
  GError* error = NULL;

  GtkBuilder* builder = gtk_builder_new();
  if (gtk_builder_add_from_file(builder, MAIN_UI_FILE, &error) == 0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return ERR;
  }

  GtkCssProvider* provider = gtk_css_provider_new();
  if (gtk_css_provider_load_from_path(provider, MAIN_STYLES_FILE, &error) ==
      0) {
    g_printerr("Error loading file: %s\n", error->message);
    g_clear_error(&error);
    return ERR;
  }
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);

  GObject* main_window = gtk_builder_get_object(builder, "main_window");
  g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_main();

  return 0;
}
