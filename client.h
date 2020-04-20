/*
 * client.h
 *
 *  Created on: Mar 2, 2020
 *      Author: alex
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include "cJSON.h"


/**
 * Sends a JSON request to socket sockfd and block until gets response.
 * Adds the field "req_id" to the request and checks that
 * the response from the socket contains the same request id.
 * Ignores responses with smaller request ids.
 *
 * Returns the response or NULL in case of an error.
 */
cJSON * send_receive(int sockfd, cJSON *req);


/**
 * Connects to the given address and port and
 * returns socket id or -1 if error.
 */
int connect_to_simple_server(char *addr, char *port);

#endif /* CLIENT_H_ */
