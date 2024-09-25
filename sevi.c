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

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"
#include "client.h"
#include "configure.h"
#include "xmalloc.h"
#include "xstring.h"

#define MAX 1024*4
#define SA struct sockaddr

// static const char *REMOTE_SERVER_ENV_NAME = "VINSNL_SERVER";
// static const char *REMOTE_SERVER_STRING = "127.0.0.1:9999";
static const char *VARIETY_ID_ENV_NAME = "LDMS_VARIETY_ID";
static const char *JOB_ID_ENV_NAME = "SLURM_JOB_ID";



static int _run_sevi(int sockfd) {

  int rc = 0;

  char * variety_id = getenv(VARIETY_ID_ENV_NAME);
  char * job_id = getenv(JOB_ID_ENV_NAME);

  printf("Job_id: %s\n", job_id);

  cJSON *req =  cJSON_CreateObject();
  cJSON_AddStringToObject(req, "type", "process_job");
  cJSON_AddStringToObject(req, "job_id", job_id);
  cJSON_AddStringToObject(req, "variety_id", variety_id);

  cJSON *resp = send_receive(sockfd, req);

  cJSON_Delete(req);

  if (resp == NULL) {
    printf("Got Null\n");
    rc = 33;
  } else {
    cJSON *payload = cJSON_GetObjectItem(resp, "status");
    printf("Got: %s\n",payload->valuestring);
    if (strcmp(payload->valuestring, "ACK") == 0) {
      rc = 0;
    } else {
      rc = 44;
    }
    cJSON_Delete(resp);
  }
  return rc;
}

static void _print_help(char *name) 
{
  printf("Arguments: \n");
  printf("  -n, --name: name of the host (overwrites config).\n");
  printf("  -p, --port: port of the host (overwrites config).\n");
  printf("  -c, --config: path to a config JSON file (overwrites env. variable).\n");
}

int sevi(int argc, char *argv[]) {
  /* initializing server address */
  char *addr = NULL;
  char *port = NULL;
  char *config_filename = NULL;

  struct option long_options[] = {
      {"name", optional_argument, NULL, 'n'},
      {"port", optional_argument, NULL, 'p'},
      {"config", optional_argument, NULL, 'c'},
      {0, 0, 0, 0}};
  char *optstring = "n:p:c:";
  int option_index = 0, c;
  while (1) {
    c = getopt_long(argc, argv, optstring, long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 'n':
        addr = optarg;
        break;
      case 'p':
        port = optarg;
        break;
      case 'c':
        config_filename = optarg;
        break;
      default:
        _print_help(argv[0]);
        exit(1);
    }
  }

  int sockfd;

  int rc = 999;

  if (!addr || !port) {
    char *addr1 = NULL;
    char *port1 = NULL;
    update_and_get_server_address(&addr1, &port1, config_filename);
    if (!addr) addr = addr1;
    if (!port) port = port1;
  }

  printf("addr: %s ; port: %s \n", addr, port);

  if (!addr || !port) {
    rc = 1;
    printf("Error: either port or address is not set \n");
  } else {
    sockfd = connect_to_simple_server(addr, port);

    if (sockfd == -1) {
      rc = 2;
      printf("Error: could not connect \n");

    } else {

      

      // run the main task
      rc = _run_sevi(sockfd);

      // close the socket
      close(sockfd);
    }
  }
  if (addr) xfree(addr);
  if (port) xfree(port);

  return rc;
}

