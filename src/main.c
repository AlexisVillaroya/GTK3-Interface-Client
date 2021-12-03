#include "client_interface/client_interface.h"

int main(int argc, char** argv) {
    init_net_functions();
    init_windows(argc, argv);
    add_styles();
    gtk_main();  
}
