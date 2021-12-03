#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_interface/client_interface.h"

int main(int argc, char** argv) {
    init_window(argc, argv);
    add_styles();
    //net_client_set_func_score_screen()
    gtk_main();  
}

