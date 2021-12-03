#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "../client_connection/net_prisoner_client.h"



void init_net_functions();
void on_window_main_destroy();
void css_set(GtkCssProvider *cssProvider, GtkWidget *g_widget);
void add_styles();
void on_betrayButton_clicked(GtkButton *button);
void on_collaborateButton_clicked(GtkButton *button);
void init_windows(int argc, char **argv);

