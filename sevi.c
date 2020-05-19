#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#include "client.h"

#define MAX 1024*4
#define SA struct sockaddr

static const char *REMOTE_SERVER_ENV_NAME = "VINSNL_SERVER";
static const char *REMOTE_SERVER_STRING = "127.0.0.1:9999";
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



int main(int argc, char *argv[]) {
  /* initializing server address */
  const char *server_string = getenv(REMOTE_SERVER_ENV_NAME);
    if (server_string == NULL)
      server_string = REMOTE_SERVER_STRING;
  char * colon = strstr(server_string, ":");
  if (!colon) {
    printf ("malformed sever string: \"%s\"\n", server_string);
    exit(1);
//<-----
  }
  char *addr = strndup(server_string, colon - server_string);
  char *port = strdup(colon+1);

  int sockfd;

  if (argc > 1) {
    addr = argv[1];
    if (argc > 2) {
      port = argv[2];
    }
  }

  sockfd = connect_to_simple_server(addr, port);

  if (sockfd == -1) {
    exit(2);
//<-----
  }

  printf("addr: %s ; port: %s \n", addr, port);

  // run the main task
  int rc = _run_sevi(sockfd);

  // close the socket
  close(sockfd);
  free(addr);
  free(port);

  return rc;
}

