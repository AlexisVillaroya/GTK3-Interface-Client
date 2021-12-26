/**
 * @file main.c
 * @author Alexis Villaroya & Wolodia Zdetovetzky
 * @brief 
 * @version 0.1
 * @date 2021-12-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "client_interface/client_interface.h"

int main(int argc, char ** argv) {
    init_net_functions();
    init_windows(argc, argv);
    gtk_main();
}
