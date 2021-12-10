/**
 * @file client_interface.h
 * @author Alexis Villaroya & Wolodia Zdetovetzky
 * @brief 
 * @version 0.1
 * @date 2021-12-04
 * 
 * @copyright Copyright (c) 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "../client_connection/net_prisoner_client.h"

/**
 * @brief initiliaze the lib needed functions 
 */
void init_net_functions();

void on_window_main_destroy();
void css_set(GtkCssProvider *cssProvider, GtkWidget *g_widget);
void add_styles();
void on_betrayButton_clicked(GtkButton *button);
void on_collaborateButton_clicked(GtkButton *button);
void init_windows(int argc, char **argv);

