#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "client_connection/client_connection.h"

GtkBuilder *builder = NULL;
GtkWidget *window;

int main(int argc, char** argv) {
    int sockfd;
    int status = 0;
    char msg[100];
    pthread_t thread;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file("glade/Interface.glade");
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_builder_connect_signals(builder, NULL);
    gtk_widget_show(window);
    gtk_main();

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

