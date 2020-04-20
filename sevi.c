#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

#include "client.h"

#define MAX 1024*4
#define PORT "9999"
#define SA struct sockaddr

static const char *VARIETY_ID_ENV_NAME = "LDMS_VARIETY_ID";
static const char *JOB_ID_ENV_NAME = "SLURM_JOB_ID";

extern char **environ;

static void _run_sevi(int sockfd) {

//  char buff[MAX+1];

  char * variety_id = getenv(VARIETY_ID_ENV_NAME);
  char * job_id = getenv(JOB_ID_ENV_NAME);

  printf("Job_id: %s", job_id);

  cJSON *req =  cJSON_CreateObject();
  cJSON_AddStringToObject(req, "type", "process_job");
  cJSON_AddStringToObject(req, "job_id", job_id);
  cJSON_AddStringToObject(req, "variety_id", variety_id);

//  int i = 1;
//  char *s = *environ;
//
//  for (; s; i++) {
//    sprintf(buff, "%i", i);
//    cJSON_AddStringToObject(req, buff, s);
//    printf("%s\n", s);
//    s = *(environ+i);
//  }

  cJSON *resp = send_receive(sockfd, req);

  cJSON_Delete(req);

  if (resp == NULL) {
    printf("Got Null\n");
  } else {
    cJSON *payload = cJSON_GetObjectItem(resp, "response");
    printf("Got: %s\n",payload->valuestring);
  }
}


int main(int argc, char *argv[]) {
  char *addr = "127.0.0.1";
  char *port = PORT;
  int sockfd;

  if (argc > 1) {
    addr = argv[1];
    if (argc > 2) {
      port = argv[2];
    }
  }

  sockfd = connect_to_simple_server(addr, port);

  if (sockfd == -1) {
    exit(1);
// <----
  }

  printf("addr: %s ; port: %s \n", addr, port);

  // run the main task
  _run_sevi(sockfd);

  // close the socket
  close(sockfd);

}

