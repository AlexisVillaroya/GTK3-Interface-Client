/**
 * @file net_prisoner_client.h
 * @author Wolodia Zdetovetzky
 * @brief all methods declaration required 
 * by a client for the prisoner dilemna
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
#include "net_prisoner_common.h"

#ifndef NET_PRISONER_CLIENT_H
#define NET_PRISONER_CLIENT_H

// ----------------------------------------------
//                     Client
// ----------------------------------------------

/**
 * @brief socket file id 
*/
extern int net_client_sockfd;

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the waiting screen
 */
extern void (*_net_client_func_waiting_screen)();

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the choice screen
 */
extern void (*_net_client_func_choice_screen)();

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the score screen of the round
 */
extern void (*_net_client_func_score_round)(net_common_round_score);

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the final score of the game
 */
extern void (*_net_client_func_score_final)(net_common_final_score);

/**
 * @brief define the function using the defined
 * one by the client to display the wainting screen
 * @param f the client function 
 */
void * net_client_set_func_waiting_screen(void (*f)());

/**
 * @brief define the function using the defined
 * one by the client to display the choice screen
 * @param f the client function 
 */
void * net_client_set_func_choice_screen(void (*f)());

/**
 * @brief define the function using the defined
 * one by the client to display the score at the end of the round
 * @param f the client function 
 */
void *net_client_set_func_score_round(void (*f)());

/**
 * @brief define the function using the defined
 * one by the client to display the score screen
 * @param f the client function 
 */
void *net_client_set_func_score_final(void (*f)());

/**
 * @brief open the connexion with the server
 * @param addrServer server address IP
 * @param port server port
 * @return if the connection with the server is OK
 */
bool net_client_init(char *addrServer, int port, int client_id);

/**
 * @brief The client is ready for the next round or to play
 */
void net_client_ready();

/**
 * @brief The client want to betray the other player
 */
void net_client_betray(ulong delay);

/**
 * @brief The client want to collaborate the other player
 */
void net_client_collab(ulong delay);

/**
 * @brief The client want to quit the game
 */
void net_client_disconnect(ulong delay);

// ----------------------------------------------
//               Private methods
// ----------------------------------------------

/**
 * @brief call the right function 
 * depends on the packet received from the server
 * @param packet the packet receive
 */
void _net_client_event(_net_common_netpacket packet);

/**
 * @brief read and display received messages
 * @param ptr 
 * @return void* 
 */
void *_net_client_threadProcess(void *ptr);

#endif /* NET_PRISONER_CLIENT_H */