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

//--------------------------------------
//               Events
//--------------------------------------
#pragma region events

// ----------- choice screen -----------
#pragma region choice_screen
/**
 * @brief Destroy the main window
 */
void on_window_main_destroy();

/**
 * @brief When the betray button is clicked ...
 * @param button 
 */
void on_betrayButton_clicked(GtkButton *button);

/**
 * @brief When the collaborate button is clicked ...
 * @param button 
 */
void on_collaborateButton_clicked(GtkButton *button);
#pragma endregion choice_screen

// ---------- settings screen ----------
#pragma region settings_screen

void on_settingsScreen_gtkButton_Valider_clicked(GtkButton *button);

#pragma endregion settings_screen

#pragma endregion events
//--------------------------------------
//              DISPLAY
//--------------------------------------
#pragma region display

/**
 * @brief 
 * @param window 
 */
void display_choice_screen();

/**
 * @brief 
 * @param waitingScreen 
 */
void display_waiting_screen();

/**
 * @brief display the screen
 * @param settingsScreen the widget
 */
void display_screen(GtkWidget *screen);

#pragma endregion display

//--------------------------------------
//                 CSS
//--------------------------------------
#pragma region css

/**
 * @brief set a widget with css 
 * @param cssProvider 
 * @param g_widget 
 */
void css_set(GtkCssProvider *cssProvider, GtkWidget *g_widget);

/**
 * @brief add style to the interface
 */
void add_styles();

#pragma endregion css

//--------------------------------------
//                 INIT
//--------------------------------------
#pragma region init

/**
 * @brief initiliaze the lib needed functions 
 */
void init_net_functions();

/**
 * @brief main initialisation  
 * @param argc 
 * @param argv 
 */
void init_windows(int argc, char **argv);

#pragma endregion init