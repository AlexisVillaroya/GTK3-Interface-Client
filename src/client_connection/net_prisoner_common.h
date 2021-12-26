/**
 * @file net_prisoner_common.h
 * @author Thomas Violent & Wolodia Zdetovetzky
 * @brief 
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <semaphore.h>

#ifndef NET_PRISONER_COMMON_H
#define NET_PRISONER_COMMON_H

// ----------------------------------------------
//                     Settings
// ----------------------------------------------

/**
 * @brief Enable logging for the net lib
 * Set this to true to allow the lib to log messages to STDOUT
 * It may be output usefull informations for debuging
 */
#define NETDEBUG true

/**
 * @brief Used to protect lib user function from multiple executions by different thread
 * if false, multithreading protection should be implemented by lib user to prevent
 * concurency issues
 */
#define THREAD_SAFETY true

/**
 * @brief  max size of the buffer 
 */
#define BUFFERSIZE 2048

/**
 * @brief Max openned connections for the server
 */
#define MAXSIMULTANEOUSCLIENTS 100

/**
 * @brief max round possible 
 */
#define MAXROUND 10

// ----------------------------------------------
//                     Common
// ----------------------------------------------

/**
 * @brief Internal.
 * Allow the lib to log message (only if NETDEBUG == true)
 * 
 * @param format même fonctionnement que printf
 * @param ... 
 */
void _net_common_dbg(const char *format, ...);

/**
 * @brief private function
 * Should init common variable used in client and server
 */
void _net_common_init();


/**
 * @brief define all the messages type exchanged between the client and the server
 */
enum _net_common_msg_type {
    
    /**
     * @brief the client betray the other
     */
    ACTION_BETRAY = 0,
    
    /**
     * @brief the client collab with the other
     */
    ACTION_COLLAB = 1,
    
    /**
     * @brief the client quit the game
     */
    ACTION_QUIT = 2,
    
    /**
     * @brief the client need to display the waiting screen before the game start
     */
    SCREEN_WAITING = 4,
    
    /**
     * @brief the client need to make a choice
     */
    SCREEN_CHOICE = 5,
    
    /**
     * @brief the client need to display the round result
     */
    SCREEN_SCORE_ROUND = 6,

    /**
     * @brief the client sent his id just after
     * initialising the connection with the server
     */
    INIT_CLIENT_ID = 7,

    /**
     * @brief the client need to display the final result
     */
    SCREEN_SCORE_FINAL = 8,

    /**
     * @brief The client may send this to client to indicate that he (the client) is
     * ready to make a choice
     */
    ACTION_READY = 9
};

/**
 * @brief player's actions
 */
enum _net_common_client_action {

    /**
     * @brief the player collab
     */
    COLLABORATE = 0,

    /**
     * @brief the player betray the other
     */
    BETRAY = 1
};

/**
 * @brief player's round score
 */
typedef struct {

    /**
     * @brief player score
     */
    int player_score;

    /**
     * @brief true if the player win
     */
    bool round_has_win;

    /**
     * @brief actual round
     */
    int round_actual;

    /**
     * @brief game total round
     */
    int round_total;  

} net_common_round_score;

typedef struct {

    /**
     * @brief the tab that contains each score of the game
     * his size is set with the defined MAXROUND, for 2 players
     */
    int result[MAXROUND][2];

} net_common_final_score;

/**
 * @brief  define the structure used to exchanged between the client and the server
 */
typedef struct {
    /**
     * @brief type of the message
     */
    enum _net_common_msg_type msg_type;

    /**
     * @brief The delay the client took to make a choice
     */
    ulong delay;

    /**
     * @brief the score at the end of each round
     */
    net_common_round_score round_score;

    /**
     * @brief the final score at the end of the game
     */
    net_common_final_score final_score;

    /**
     * @brief unique client id defined
     * int the client config file
     */
    int client_id;

} _net_common_netpacket;

#endif /* NET_PRISONER_COMMON_H */