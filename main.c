#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "clientcxnmanager.h"

GtkBuilder *builder = NULL;

int timer_id = 0;
int elapsed_time = 0;

/**
 * Fermeture de la fenetre
 * cette fonction est invoke par le gestionnaire d'evt
 */
void on_window_main_destroy() {
    printf("quitting\n ");
    gtk_main_quit();
}

/**
 * Fonction invoque lors du click sur le bouton 'Yes'
 */
void on_button_click() {
    printf("bouton 'Yes' clicked\n");
    GtkEntry *texte = GTK_ENTRY(gtk_builder_get_object(builder, "texte"));
    gchar *data = (gchar *) gtk_entry_get_text(texte);
    GtkEntry *echo = GTK_ENTRY(gtk_builder_get_object(builder, "echo"));
    gtk_entry_set_text(echo, data);
}

int timer_handler() {
    elapsed_time++;
    char txt[100];
    printf("timer running, time : %d\n", elapsed_time);
    GtkLabel *timelabel = GTK_LABEL(gtk_builder_get_object(builder, "time_display"));
    snprintf(txt, 100, "%04i", elapsed_time);
    gtk_label_set_text(timelabel, txt);
    return 1;
}

/**
 * Gestion du bouton toggle_one avec demarrage/arret d'un timer
 * @param widget  le widget qui a emit l'evt
 * @param data des donnees supplementatire
 */
void on_toogle(GtkWidget *widget, gpointer data) {
    GtkToggleButton *btn = GTK_TOGGLE_BUTTON(widget);
    gchar *name = (gchar *) gtk_widget_get_name(widget);
    gboolean status = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
    printf("toggle %s pressed, status : %d\n", name, status);
    //Activation du timer s'il etait arrete
    if (timer_id == 0) {
        timer_id = g_timeout_add(1000, (GSourceFunc) timer_handler, NULL);
    } else {
        g_source_remove(timer_id);
        timer_id = 0;
    }
}

void on_cancel() {
    GtkWidget *message_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK_CANCEL,
            "This action will cause the universe to stop existing.");
    //cf : http://refspecs.linuxbase.org/gtk/2.6/gtk/GtkMessageDialog.html
    //https://developer.gnome.org/gnome-devel-demos/stable/messagedialog.c.html.en
    unsigned int pressed = gtk_dialog_run(GTK_DIALOG(message_dialog));
    if (pressed == GTK_RESPONSE_OK) {
        printf("OK Pressed \n");
        printf("quitting\n ");
        gtk_widget_destroy(message_dialog);
        gtk_main_quit();
    } else {
        printf("CANCEL Pressed \n");
        gtk_widget_destroy(message_dialog);
    }
}

/*
 * 
 */
int main(int argc, char** argv) {

    GtkWidget *win;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/Interface.glade");
    win = GTK_WIDGET(gtk_builder_get_object(builder, "app_win"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(win);
    gtk_main();

    int sockfd;
    int status = 0;
    char msg[100];
    pthread_t thread;

    sockfd = open_connection();

    strcpy(msg, "Hello from Xeon"); //Xeon is the name of the this client
    printf("sending : %s\n", msg);
    write(sockfd, msg, strlen(msg));

    //Creation d'un pthread de lecture
    pthread_create(&thread, 0, threadProcess, &sockfd);
    //write(connection->sock,"Main APP Still running",15);
    pthread_detach(thread);
    do {
        fgets(msg, 100, stdin);
        //printf("sending : %s\n", msg);
        status = write(sockfd, msg, strlen(msg));
        //memset(msg,'\0',100);
    } while (status != -1);

    return (EXIT_SUCCESS);
}

