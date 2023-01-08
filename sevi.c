#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cJSON.h"
#include "client.h"
#include "configure.h"

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



int main(int argc, char *argv[]) {
  /* initializing server address */
  char *addr = NULL;
  char *port = NULL;


  int sockfd;

  int rc = 999;

  update_and_get_server_address(&addr, &port);

  if (argc > 1) {
    if (addr) free(addr);
    addr = argv[1];
    if (argc > 2) {
      if (port) free (port);
      port = argv[2];
    }
  }

  if (!addr || !port) {
    rc = 1;
  } else {
    sockfd = connect_to_simple_server(addr, port);

    if (sockfd == -1) {
      rc = 2;
    } else {

      printf("addr: %s ; port: %s \n", addr, port);

      // run the main task
      rc = _run_sevi(sockfd);

      // close the socket
      close(sockfd);
    }
  }
  if (addr) free(addr);
  if (port) free(port);

  return rc;
}

