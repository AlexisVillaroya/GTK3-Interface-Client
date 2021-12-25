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

// choice screen
GtkWidget *ChoiceScreen;
GtkWidget *choiceScreen_label_round;
GtkWidget *choiceScreen_label_sanction;
GtkButton *choiceScreen_gtkButton_betray;
GtkButton *choiceScreen_gtkButton_collaboration;
GtkWidget *choiceScreen_textView_result;

// waiting screen
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
void on_window_main_destroy()
{
    puts("quitting");
    net_client_disconnect(10);
    gtk_main_quit();
}

/**
 * @brief When the betray button is clicked ...
 * @param button
 */
void on_betrayButton_clicked(GtkButton *button)
{
    puts("betray !");
    net_client_betray(10);
}

/**
 * @brief When the collaborate button is clicked ...
 * @param button
 */
void on_collaborateButton_clicked(GtkButton *button)
{
    puts("collaboration !");
    net_client_collab(10);
}
#pragma endregion choice_screen

// ---------- settings screen ----------
#pragma region settings_screen

void on_settingsScreen_gtkButton_Valider_clicked(GtkButton *button)
{
    gchar *serverIP = gtk_entry_get_text(settingsScreen_GtkEntry_serverIP);
    gint serverPort = atoi(gtk_entry_get_text(settingsScreen_GtkEntry_serverPort));
    gint clientID = atoi(gtk_entry_get_text(settingsScreen_GtkEntry_clientID));

    if (!net_client_init(serverIP, serverPort, clientID))
    {
        gtk_label_set_label(settingsScreen_gtkLabel_errors, "Erreur : connexion impossible avec le serveur. Vérifiez votre saisie.");
        puts("CLIENT : Erreur de saisie ou de connexion avec le serveur.");
        perror("ERROR ");
    }
}
#pragma endregion settings_screen

#pragma endregion events

//--------------------------------------
//              DISPLAY
//--------------------------------------
#pragma region display

/**
 * @brief hide waiting screen and display the choice screen
 */
void display_choice_screen()
{

    gtk_builder_connect_signals(builder, NULL);
    waitingScreen = GTK_WIDGET(gtk_builder_get_object(builder, "waitingScreen"));

    ChoiceScreen = GTK_WIDGET(gtk_builder_get_object(builder, "ChoiceScreen"));
    g_signal_connect(ChoiceScreen, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    choiceScreen_label_round = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_label_round"));
    choiceScreen_label_sanction = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_label_sanction"));
    choiceScreen_gtkButton_betray = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_gtkButton_betray"));
    choiceScreen_gtkButton_collaboration = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_gtkButton_collaboration"));

    gtk_widget_hide(waitingScreen);

    gtk_widget_show(ChoiceScreen);
}

/**
 * @brief hide setting screen and display the waiting screen
 */
void display_waiting_screen()
{

    gtk_builder_connect_signals(builder, NULL);

    settingsScreen = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen"));

    waitingScreen = GTK_WIDGET(gtk_builder_get_object(builder, "waitingScreen"));
    waitingSpinner = GTK_WIDGET(gtk_builder_get_object(builder, "waitingSpinner"));
    waitingLabel = GTK_WIDGET(gtk_builder_get_object(builder, "waitingLabel"));
    g_signal_connect(waitingScreen, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_hide(settingsScreen);

    gtk_widget_show(waitingScreen);
}

/**
 * @brief display the screen
 * @param settingsScreen the widget
 */
void display_screen(GtkWidget *screen)
{
    gtk_widget_show_all(screen);
}

/**
 * @brief handle round score at the end of the round
 * @param round_score contain all informations needed
 */
void handle_round_score(net_common_round_score round_score)
{
    // view init
    gtk_builder_connect_signals(builder, NULL);
    ChoiceScreen = GTK_WIDGET(gtk_builder_get_object(builder, "ChoiceScreen"));
    choiceScreen_textView_result = GTK_WIDGET(gtk_builder_get_object(builder, "choice_screen_textView_result"));

    // creation of the result string
    char *result = malloc(sizeof(char) * 64);
    sprintf(result, "Round result %d/%d :\n\t", round_score.round_actual, round_score.round_total);

    char *tmp = malloc(sizeof(char) * 15);
    if (round_score.round_has_win)
    {
        tmp = "You win !\n\t";
    }
    else
    {
        tmp = "You loose !\n\t";
    }
    strcat(result, tmp);

    char *score = malloc(sizeof(char) * 20);
    sprintf(score, "Your score : %d", round_score.player_score);
    strcat(result, score);

    char *round = malloc(sizeof(char) * 15);
    sprintf(round, "Round :\n%d/%d", round_score.round_actual, round_score.round_total);
    char *sanction = malloc(sizeof(char) * 15);
    sprintf(sanction, "Sanction :\n%d", round_score.player_score);

    // display
    gtk_label_set_text(choiceScreen_textView_result, result);
    gtk_label_set_text(choiceScreen_label_round, round);
    gtk_label_set_text(choiceScreen_label_sanction, sanction);
}

/**
 * @brief handle final score at the end of the game
 * @param final_score contain all informations needed
 */
void handle_final_score(net_common_final_score final_score)
{
    // view init
    gtk_builder_connect_signals(builder, NULL);
    ChoiceScreen = GTK_WIDGET(gtk_builder_get_object(builder, "ChoiceScreen"));
    choiceScreen_textView_result = GTK_WIDGET(gtk_builder_get_object(builder, "choice_screen_textView_result"));

    // memory allocation
    int total_J1 = 0;
    int total_J2 = 0;
    char *result = malloc(sizeof(char) * 300);
    char *total = malloc(sizeof(char) * 25);

    // tab
    strcpy(result, "Final result :\n|\tRound\t|\tYou\t|\tP2\t|\n");
    for (int round = 0; round < MAXROUND; round++)
    {
        char *rnd_str = malloc(sizeof(char) * 20);
        sprintf(rnd_str, "|\t%d\t|\t%d\t|\t%d\t|\n", round + 1, final_score.result[round][0], final_score.result[round][1]);
        strcat(result, rnd_str);
        free(rnd_str);

        total_J1 += final_score.result[round][0];
        total_J2 += final_score.result[round][1];
    }

    // total
    sprintf(total, "|\tTotal\t|\t%d\t|\t%d\t|\n", total_J1, total_J2);
    strcat(result, total);

    // labels
    char *round = malloc(sizeof(char) * 15);
    sprintf(round, "Round :\n%d/%d", MAXROUND, MAXROUND);
    char *sanction = malloc(sizeof(char) * 15);
    sprintf(sanction, "Sanction :\n%d", total_J1);

    // display
    gtk_label_set_text(choiceScreen_textView_result, result);
    gtk_label_set_text(choiceScreen_label_round, round);
    gtk_label_set_text(choiceScreen_label_sanction, sanction);
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
void css_set(GtkCssProvider *cssProvider, GtkWidget *g_widget)
{
    GtkStyleContext *context = gtk_widget_get_style_context(g_widget);

    gtk_style_context_add_provider(
        context,
        GTK_STYLE_PROVIDER(cssProvider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);
}

/**
 * @brief add style to the interface
 */
void add_styles()
{
    GtkCssProvider *cssProvider1;
    cssProvider1 = gtk_css_provider_new();

    // load the provider
    gtk_css_provider_load_from_path(cssProvider1, "include/styles/gtk.css", NULL);
    css_set(cssProvider1, ChoiceScreen);
    css_set(cssProvider1, choiceScreen_gtkButton_betray);
    css_set(cssProvider1, choiceScreen_gtkButton_collaboration);
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
void init_net_functions()
{
    net_client_set_func_waiting_screen(display_waiting_screen);
    net_client_set_func_choice_screen(display_choice_screen);
    net_client_set_func_score_round(handle_round_score);
    net_client_set_func_score_final(handle_final_score);
}

/**
 * @brief main initialisation
 * s
 * @param argc
 * @param argv
 */
void init_windows(int argc, char **argv)
{
    gtk_init(&argc, &argv);

    // establish contact with xml code to adjust widget settings
    // builder is the pointer to the digest xml file
    builder = gtk_builder_new_from_file("glade/Interface.glade");
    gtk_builder_connect_signals(builder, NULL);

    // choice screen
    // GTK_WIDGET is a cast here because windows is a widget
    ChoiceScreen = GTK_WIDGET(gtk_builder_get_object(builder, "ChoiceScreen"));
    g_signal_connect(ChoiceScreen, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    choiceScreen_label_round = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_label_round"));
    choiceScreen_label_sanction = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_label_sanction"));
    choiceScreen_gtkButton_betray = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_gtkButton_betray"));
    choiceScreen_gtkButton_collaboration = GTK_WIDGET(gtk_builder_get_object(builder, "choiceScreen_gtkButton_collaboration"));

    // waiting screen
    waitingScreen = GTK_WIDGET(gtk_builder_get_object(builder, "waitingScreen"));
    g_signal_connect(waitingScreen, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    waitingSpinner = GTK_WIDGET(gtk_builder_get_object(builder, "waitingSpinner"));
    waitingLabel = GTK_WIDGET(gtk_builder_get_object(builder, "waitingLabel"));

    // settings screen
    settingsScreen = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen"));
    g_signal_connect(settingsScreen, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    settingsScreen_GtkEntry_serverIP = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_GtkEntry_serverIP"));
    settingsScreen_GtkEntry_serverPort = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkEntry_serverPort"));
    settingsScreen_GtkEntry_clientID = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkEntry_clientID"));
    settingsScreen_gtkButton_valider = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkButton_Valider"));
    settingsScreen_gtkLabel_errors = GTK_WIDGET(gtk_builder_get_object(builder, "settingsScreen_gtkLabel_errors"));

    // default
    gtk_entry_set_text(settingsScreen_GtkEntry_serverIP, "0.0.0.0");
    gtk_entry_set_text(settingsScreen_GtkEntry_serverPort, "7799");
    // gtk_entry_set_text(settingsScreen_GtkEntry_clientID, "1");

    // set CSS style
    add_styles();

    // display_screen(settingsScreen);
    gtk_widget_show(settingsScreen);
}
#pragma endregion init
