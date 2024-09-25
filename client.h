/**
 * Copyright (c) 2024 Alexander Goponenko. University of Central Florida.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * “Software”), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
