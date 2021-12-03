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

//--------------------------------------
//               Events
//--------------------------------------

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
    net_client_collab(10);
}

//--------------------------------------
//              DISPLAY
//--------------------------------------

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

//--------------------------------------
//                 CSS
//--------------------------------------

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

//--------------------------------------
//                 INIT
//--------------------------------------

/**
 * @brief delay between server and client responses
 */
ulong delay;

/**
 * @brief init functions used by the lib
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

    //net_client_set_func_choice_screen();
    //establish contact with xml code to adjust widget settings
    //builder is the pointer to the digest xml file
    builder = gtk_builder_new_from_file("glade/Interface.glade");
    //GTK_WIDGET is a cast here because windows is a widget
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);


    betrayButton = GTK_WIDGET(gtk_builder_get_object(builder, "betrayButton"));
    collaborateButton = GTK_WIDGET(gtk_builder_get_object(builder, "collaborateButton"));
    buttonFree = GTK_WIDGET(gtk_builder_get_object(builder, "buttonFree"));

    waitingScreen = GTK_WIDGET(gtk_builder_get_object(builder, "waitingScreen"));
    waitingSpinner = GTK_WIDGET(gtk_builder_get_object(builder, "waitingSpinner"));
    waitingLabel = GTK_WIDGET(gtk_builder_get_object(builder, "waitingLabel"));

   display_waiting_screen(waitingScreen);
}