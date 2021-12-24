/**
 * @file net_prisoner_client.c
 * @author Wolodia Zdetovetzky
 * @brief all methods implementation required 
 * by a client for the prisoner dilemna
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "net_prisoner_client.h"

// ----------------------------------------------
//                     Client
// ----------------------------------------------
#pragma region Client

/**
 * @brief client socket file id
 */
int net_client_sockfd;

/**
 * @brief the unique client id
 */
int net_client_id;

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the waiting screen
 */
void (*_net_client_func_waiting_screen)();

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the choice screen
 */
void (*_net_client_func_choice_screen)();

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the score screen of the round
 */
void (*_net_client_func_score_screen)(net_common_round_score);

/**
 * @brief the function used by the library
 * refering to the defined one by the client
 * to display the final score of the game
 */
void (*_net_client_func_score_final)(net_common_final_score);

/**
 * @brief define the function using the defined
 * one by the client to display the wainting screen
 * @param f the client function 
 */
void *net_client_set_func_waiting_screen(void (*f)())
{
    _net_client_func_waiting_screen = f;
}

/**
 * @brief define the function using the defined
 * one by the client to display the choice screen
 * @param f the client function 
 */
void *net_client_set_func_choice_screen(void (*f)())
{
    _net_client_func_choice_screen = f;
}

/**
 * @brief define the function using the defined
 * one by the client to display the score screen
 * @param f the client function 
 */
void *net_client_set_func_score_final(void (*f)())
{
    _net_client_func_score_final = f;
}

/**
 * @brief define the function using the defined
 * one by the client to display the score screen
 * @param f the client function 
 */
void *net_client_set_func_score_screen(void (*f)())
{
    _net_client_func_score_screen = f;
}

/**
 * @brief call the right function 
 * depends on the packet received from the server
 * @param packet the packet receive
 */
void _net_client_event(_net_common_netpacket packet)
{

    switch (packet.msg_type)
    {
    case SCREEN_WAITING:
        _net_common_dbg("Client %d received SCREEN_WAITING from server\n", net_client_id);
        (*_net_client_func_waiting_screen)();
        break;

    case SCREEN_CHOICE:
        _net_common_dbg("Client %d received SCREEN_CHOICE from server\n", net_client_id);
        (*_net_client_func_choice_screen)();
        break;

    case SCREEN_SCORE_ROUND:
        _net_common_dbg("Client %d received SCREEN_SCORE_ROUND from server\n", net_client_id);
        (*_net_client_func_score_screen)(packet.round_score);
        break;

    case SCREEN_SCORE_FINAL:
        _net_common_dbg("Client %d received SCREEN_SCORE_FINAL from server\n", net_client_id);
        (*_net_client_func_score_final)(packet.final_score);
        break;

    default:
        _net_common_dbg("Unknown message type, do you have the latest version of the lib ?\n");
        break;
    }
}

/**
 * @brief read and display received messages
 * @param ptr the net_client_sockfd
 * @return void* 
 */
void *_net_client_threadProcess(void *ptr)
{
    _net_common_netpacket packet;

    net_client_sockfd = *((int *)ptr);
    int len;
    while ((len = read(net_client_sockfd, &packet, sizeof(packet))) != 0)
    {   
        _net_common_dbg("client %d receive %d bits\n", net_client_id, sizeof(packet));
        _net_client_event(packet);
        //memset(&packet, '\0', len);
    }
    close(net_client_sockfd);
    _net_common_dbg("client pthread ended, len=%d\n", len);
}

/**
 * @brief open the connexion with the server
 * @param addrServer server address IP
 * @param port server port
 */
bool net_client_init(char *addrServer, int port, int client_id)
{
    struct sockaddr_in serverAddr;
    pthread_t thread;

    // Create the socket.
    net_client_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure settings of the server address
    // Address family is Internet
    serverAddr.sin_family = AF_INET;

    //Set port number, using htons function
    serverAddr.sin_port = htons(port);

    //Set IP address to localhost
    serverAddr.sin_addr.s_addr = inet_addr(addrServer);

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    //connect(net_client_sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    
    //Connect the socket to the server using the address
    if (connect(net_client_sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) != 0)
    {
        _net_common_dbg("\nFail to connect to server\n");
        return false;
    };
    

    // init the client id
    _net_common_netpacket packet;
    packet.msg_type = INIT_CLIENT_ID;
    packet.client_id = client_id;
    write(net_client_sockfd, &packet, sizeof(packet));
    _net_common_dbg("the client sent his id : %d\n", net_client_id);

    // reading pthread creation
    pthread_create(&thread, 0, _net_client_threadProcess, &net_client_sockfd);
    pthread_detach(thread);

    return true;
}

/**
 * @brief The client is ready for the next round
 */
void net_client_ready()
{
    _net_common_netpacket packet;
    packet.msg_type = ACTION_READY;
    packet.client_id = net_client_id;
    write(net_client_sockfd, &packet, sizeof(packet));
    _net_common_dbg("%d is ready\n", net_client_id);
}

/**
 * @brief The client want to betray the other player
 */
void net_client_betray(ulong delay)
{
    _net_common_netpacket packet;
    packet.msg_type = ACTION_BETRAY;
    packet.client_id = net_client_id;
    packet.delay = delay;
    write(net_client_sockfd, &packet, sizeof(packet));
    _net_common_dbg("%d want to betray\n", net_client_id);
}

/**
 * @brief The client want to collaborate the other player
 */
void net_client_collab(ulong delay)
{
    _net_common_netpacket packet;
    packet.msg_type = ACTION_COLLAB;
    packet.client_id = net_client_id;
    packet.delay = delay;
    write(net_client_sockfd, &packet, sizeof(packet));
    _net_common_dbg("%d want to collab\n", net_client_id);
}

/**
 * @brief The client want to quit the game
 */
void net_client_disconnect(ulong delay)
{
    _net_common_netpacket packet;
    packet.msg_type = ACTION_QUIT;
    packet.client_id = net_client_id;
    packet.delay = delay;
    write(net_client_sockfd, &packet, sizeof(packet));
    _net_common_dbg("%d want to quit\n", net_client_id);
}
#pragma endregion Client