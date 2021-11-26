#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_interface/client_interface.h"

int main(int argc, char** argv) {
    init_window(argc, argv);
    add_styles();
    //init_connection();
    gtk_main();  
}

