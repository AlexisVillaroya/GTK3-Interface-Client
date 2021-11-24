#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "client_interface.h"   

GtkBuilder *builder;
GtkWidget *window;
GtkWidget *betrayButton;
GtkWidget *collaborateButton;

/**
 * @brief Destroy the main window
 * 
 */

void on_window_main_destroy() {
    printf("quitting\n ");
    gtk_main_quit();
}

/**
 * @brief When the betray button is clicked ...
 * 
 * @param button 
 */

void on_betrayButton_clicked(GtkButton *button){
    printf("trahison !\n");
}

/**
 * @brief When the collaborate button is clicked ...
 * 
 * @param button 
 */

void on_collaborateButton_clicked(GtkButton *button){
    printf("collaboration !\n");
}

/**
 * @brief Initialisation of the main window
 * 
 * @param argc 
 * @param argv 
 */

void init_window(int argc, char **argv){
    gtk_init(&argc, &argv);

    //establish contact with xml code to adjust widget settings

    //builder is the pointer to the digest xml file
    builder = gtk_builder_new_from_file("glade/Interface.glade");
    //GTK_WIDGET is a cast here because windows is a widget
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);

    betrayButton = GTK_WIDGET(gtk_builder_get_object(builder, "betrayButton"));
    collaborateButton = GTK_WIDGET(gtk_builder_get_object(builder, "collaborateButton"));
    gtk_widget_show(window);
}