#include "../s21_viewer.h"

gboolean read_err(gpointer user_data) {
  GObject *pollable_stream = user_data;

  g_print("Read Stream\n");
  char *buffer = g_malloc(500);
  memset(buffer, '\0', 500);
  gssize bytes_read = 0;

  bytes_read = g_pollable_input_stream_read_nonblocking(
      (GPollableInputStream *)pollable_stream, buffer, 499, NULL, NULL);
  if (bytes_read > 0) {
    g_print("Error From Gnuplot: %s", buffer);
  }
  if (bytes_read == -1) {
    g_print("Input Stream Error.");
  }
  g_input_stream_close((GInputStream *)pollable_stream, NULL, NULL);
  g_free(buffer);

  return FALSE;
}

void gnuplot_finished(GObject *source_object, GAsyncResult *res,
                      GtkWidget *image) {
  UNUSED(res);

  gtk_image_set_from_file(GTK_IMAGE(image), IMAGE_OUTPUT);
  gtk_widget_queue_draw(image);
  if (g_subprocess_get_successful(G_SUBPROCESS(source_object))) {
    g_print("Gnuplot Success\n");
  }
  g_object_unref(source_object);
  g_print("Gnuplot Finished\n");
}

void call_gnuplot(gchar *cmd, gchar *script, GtkWidget *plot_image) {
  GSubprocess *sub_process = g_subprocess_new(
      G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE, NULL, cmd,
      NULL);
  GInputStream *err_stream = g_subprocess_get_stderr_pipe(sub_process);
  GSource *source = g_pollable_input_stream_create_source(
      (GPollableInputStream *)err_stream, NULL);
  g_source_attach(source, NULL);
  g_source_set_callback(source, (GSourceFunc)read_err, NULL, NULL);
  g_subprocess_wait_async(sub_process, NULL,
                          (GAsyncReadyCallback)gnuplot_finished, plot_image);
  GOutputStream *stream = g_subprocess_get_stdin_pipe(sub_process);
  g_output_stream_write(stream, script, strlen(script) + 1, NULL, NULL);
  g_output_stream_flush(stream, NULL, NULL);
  g_output_stream_close(stream, NULL, NULL);
}

void gnuplot_call_wrapper(GtkWidget *plot_image) {
  gint width = gtk_widget_get_allocated_width(plot_image);
  gint height = gtk_widget_get_allocated_height(plot_image);

  gchar *cmd = g_strdup("/usr/bin/gnuplot");
  gchar *script =
      g_strdup_printf(GNUPLOT_FORMAT, width, height, IMAGE_OUTPUT, POINTS_FILE);
  g_print("%s\n", script);

  call_gnuplot(cmd, script, plot_image);

  g_free(cmd);
  g_free(script);
}
