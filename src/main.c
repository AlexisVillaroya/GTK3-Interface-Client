#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_connection/client_connection.h"
#include "client_interface/client_interface.h"

int main(int argc, char** argv) {
    init_window(argc, argv);
    //init_connection();
    gtk_main();  
}

