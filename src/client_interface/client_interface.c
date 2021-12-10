/**
 * @file client_interface.c
 * @author Alexis Villaroya & Wolodia Zdetovetzky
 * @brief 
 * @version 0.1
 * @date 2021-12-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "client_interface.h" 
#include "../client_connection/net_prisoner_client.h"

// init all widgets used
GtkBuilder *builder;
GtkWidget *window;
GtkButton *betrayButton;
GtkWidget *collaborateButton;
GtkWidget *buttonFree;
GtkWidget *buttonSixMonth;
GtkWidget *buttonFiveYears;
GtkWidget *buttonTenYears;
GtkWidget *waitingScreen;
GtkWidget *waitingSpinner;
GtkWidget *waitingLabel;

// settings screen
GtkWidget *settingsScreen;
GtkWidget *settingsScreen_GtkEntry_serverIP;
GtkWidget *settingsScreen_GtkEntry_serverPort;
GtkWidget *settingsScreen_GtkEntry_clientID;
GtkWidget *settingsScreen_gtkButton_valider;
GtkWidget *settingsScreen_gtkLabel_errors;

//--------------------------------------
//               Events
//--------------------------------------
#pragma region events

// ----------- choice screen -----------
#pragma region choice_screen
/**
 * @brief Destroy the main window
 */
void on_window_main_destroy() {
    printf("quitting\n ");
    net_client_disconnect(10);
    gtk_main_quit();
}

/**
 * @brief When the betray button is clicked ...
 * @param button 
 */
void on_betrayButton_clicked(GtkButton *button){
    printf("trahison !\n");
    net_client_betray(10);
}

/**
 * @brief When the collaborate button is clicked ...
 * @param button 
 */
void on_collaborateButton_clicked(GtkButton *button){
    printf("collaboration !\n");
    //net_client_collab();
    net_client_collab(10);
}
#pragma endregion choice_screen

// ---------- settings screen ----------
#pragma region settings_screen


void on_settingsScreen_gtkButton_Valider_clicked(GtkButton *button) {

    gchar * serverIP = gtk_entry_get_text(settingsScreen_GtkEntry_serverIP);
    gint serverPort = gtk_entry_get_text(settingsScreen_GtkEntry_serverPort);
    gint clientID = gtk_entry_get_text(settingsScreen_GtkEntry_clientID);

    if (!net_client_init(serverIP, serverPort, clientID)) {
        gtk_label_set_label(settingsScreen_gtkLabel_errors, "Erreur : connexion impossible avec le serveur. Vérifiez votre saisie.");
        puts("CLIENT : Erreur de saisie ou de connexion avec le serveur.");
    }
}
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
void display_main_window(GtkWidget *window){
    gtk_widget_show_all(window);
}

/**
 * @brief 
 * @param waitingScreen 
 */
void display_waiting_screen(GtkWidget *waitingScreen){
    gtk_widget_show_all(waitingScreen);
}

/**
 * @brief display the screen
 * @param settingsScreen the widget
 */
void display_screen(GtkWidget *screen) {
    gtk_widget_show_all(screen);
}
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
void css_set(GtkCssProvider *cssProvider, GtkWidget *g_widget){
    GtkStyleContext *context = gtk_widget_get_style_context(g_widget);

    gtk_style_context_add_provider(context, 
    GTK_STYLE_PROVIDER(cssProvider),
    GTK_STYLE_PROVIDER_PRIORITY_USER);
}

/**
 * @brief add style to the interface
 */
void add_styles(){
    GtkCssProvider *cssProvider1;
    cssProvider1 = gtk_css_provider_new();

    //load the provider
    gtk_css_provider_load_from_path(cssProvider1, "include/styles/gtk.css", NULL);
    css_set(cssProvider1, window);
    css_set(cssProvider1, betrayButton);
}
#pragma endregion css

//--------------------------------------
//                 INIT
//--------------------------------------
#pragma region init

/**
 * @brief delay between server and client responses
 */
ulong delay;

/**
 * @brief initiliaze the lib needed functions 
 */
void init_net_functions() {
    net_client_set_func_waiting_screen(display_waiting_screen);
    net_client_set_func_choice_screen(display_main_window);
}

/**
 * @brief Initialisation of the main windows
 * @param argc 
 * @param argv 
 */
void init_windows(int argc, char **argv){
    gtk_init(&argc, &argv);

    //establish contact with xml code to adjust widget settings
    //builder is the pointer to the digest xml file
    builder = gtk_builder_new_from_file("glade/Interface.glade");

    // choice screen
    //GTK_WIDGET is a cast here because windows is a widget
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    waitingScreen = GTK_WIDGET(gtk_builder_get_object(builder, "waitingScreen"));
    g_signal_connect(waitingScreen, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);
    betrayButton = GTK_WIDGET(gtk_builder_get_object(builder, "betrayButton"));
    collaborateButton = GTK_WIDGET(gtk_builder_get_object(builder, "collaborateButton"));
    buttonFree = GTK_WIDGET(gtk_builder_get_object(builder, "buttonFree"));

    // waiting screen
    waitingScreen = GTK_WIDGET(gtk_builder_get_object(builder, "waitingScreen"));
    waitingSpinner = GTK_WIDGET(gtk_builder_get_object(builder, "waitingSpinner"));
    waitingLabel = GTK_WIDGET(gtk_builder_get_object(builder, "waitingLabel"));

    // settings screen
    settingsScreen  = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen"));
    settingsScreen_GtkEntry_serverIP  = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_GtkEntry_serverIP"));
    settingsScreen_GtkEntry_serverPort  = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkEntry_serverPort"));
    settingsScreen_GtkEntry_clientID  = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkEntry_clientID"));
    settingsScreen_gtkButton_valider  = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkButton_Valider"));
    settingsScreen_gtkLabel_errors  = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkLabel_errors"));
    // default
    gtk_entry_set_text(settingsScreen_GtkEntry_serverIP, "0.0.0.0");
    gtk_entry_set_text(settingsScreen_GtkEntry_serverPort, "7799");
    //gtk_entry_set_text(settingsScreen_GtkEntry_clientID, "1");

    display_screen(settingsScreen);
}
#pragma endregion init
