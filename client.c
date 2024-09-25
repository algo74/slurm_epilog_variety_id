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

#include "client.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "cJSON.h"

#define MAX 1024*4
//#define SA struct sockaddr
#define ID_LEN 32



cJSON *
send_receive(int sockfd, cJSON *req)
{
  static unsigned int count = 0;
  static char req_id[ID_LEN+1];
  static char buff[MAX+1];
  cJSON *res = NULL;
  count++;
  if (req == NULL) {
    fprintf(stderr, "Error: empty request JSON\n");
    //TODO: process error
    goto Cleanup01;
  }
  snprintf(req_id, ID_LEN, "%d", count);
  if (cJSON_AddStringToObject(req, "req_id", req_id) == NULL) {
    fprintf(stderr, "Error adding req_id\n");
    //TODO: process error
    goto Cleanup01;
  }
  char *req_str = cJSON_PrintUnformatted(req);
  if (req_str == NULL) {
    fprintf(stderr, "Failed to print JSON.\n");
    //TODO: process error
    goto Cleanup01;
  }

  write(sockfd, req_str, strlen(req_str));
  free(req_str);

  for (;;) {
    memset(buff, 0, sizeof(buff));
    read(sockfd, buff, MAX);
    printf("From Server : %s\n", buff);
    cJSON *resp = cJSON_Parse(buff);
    if (resp == NULL) {
      const char *error_ptr = cJSON_GetErrorPtr();
      if (error_ptr != NULL) {
        fprintf(stderr, "Error before: %s\n", error_ptr);
      } else {
        fprintf(stderr, "Error parsing\n");
      }
      break;
    }
    cJSON* id = cJSON_GetObjectItem(resp, "req_id");
    if (id == NULL || !cJSON_IsString(id) || id->valuestring == NULL) {
      fprintf(stderr, "Error: no id\n");
      cJSON_Delete(resp);
      break;
    }
    if (strcmp(id->valuestring, req_id) != 0) {
      fprintf(stderr, "Error: wrong id: sent \"%s\" got \"%s\"\n", req_id, id->valuestring);
      int resp_id = atoi(id->valuestring);
      cJSON_Delete(resp);
      if(resp_id && resp_id < count) {
        continue;
      }
      break;
    }
    res = resp;
    break;
  }

Cleanup01:

  return res;
}



int connect_to_simple_server(char *addr, char *port) {
  int sockfd;
  // resolve address
  struct addrinfo hint = { 0 };
  hint.ai_flags = 0;
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = IPPROTO_TCP;

  struct addrinfo *addrs = NULL;
  int ret = getaddrinfo(addr, port, &hint, &addrs);
  if (ret != 0) {
    printf("Could not resolve address %s:%s\n", addr, port);
    return -1;
// <------
  }

  // check resolved addresses by connecting until success
  for (struct addrinfo *addr = addrs; addr != NULL; addr = addr->ai_next) {
    // log ip address
    char *s = NULL;
    struct sockaddr *res = addr->ai_addr;
    switch (res->sa_family) {
      case AF_INET: {
        struct sockaddr_in *addr_in = (struct sockaddr_in*) res;
        s = (char*) malloc(INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(addr_in->sin_addr), s, INET_ADDRSTRLEN);
        break;
      }
      case AF_INET6: {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6*) res;
        s = (char*) malloc(INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &(addr_in6->sin6_addr), s, INET6_ADDRSTRLEN);
        break;
      }
      default:
        break;
    }
    printf("IP address: %s\n", s);
    free(s);

    // create socket
    sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sockfd == -1) {
      printf("Could not create socket\n");
    } else {
      if (connect(sockfd, addr->ai_addr, addr->ai_addrlen) == 0) {
        // connected
        break;
      } else {
        close(sockfd);
        sockfd = -1;
      }
    }
    if (addr->ai_next == NULL) {
      printf("connect error\n");
    }
  }

  freeaddrinfo(addrs);

  return sockfd;

}
