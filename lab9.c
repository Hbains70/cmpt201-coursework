#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUF_SIZE 64
#define ADDR "127.0.0.1"

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

/*
Questions to answer at top of client.c:
 1. What si the address of the server it is trying to connect to (IP adddress
 and port number).
    It is 127.0.0.1 the local host on port 8000
 2. Is it UDP or TCP? How do you know?
    Its TCP since it utilizes the SOCK_STREAM in the call of socket(), which is
    based for TCP
 3. The client is going to sen some data to the server. Where does it get this data from?
    How can you tell in the code?
    This is from the standard input like stdin, where the loop calls
    read(STDIN_FILENO, buf, BUF_SIZE)
 4. How does the client program end? How can you tell that in the code?
    Its when the read() from stdin gives back a value less than or equal to 1 returning.
    After the socket closes and will call exit(EXIT_SUCCESS).
*/
int main() {
  struct sockaddr_in addr;
  int sfd;
  ssize_t num_read;
  char buf[BUF_SIZE];

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, ADDR, &addr.sin_addr) <= 0) {
    handle_error("inet_pton");
  }

  int res = connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));
  if (res == -1) {
    handle_error("connect");
  }

  while ((num_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 1) {
    if (write(sfd, buf, num_read) != num_read) {
      handle_error("write");
    }
    printf("Just sent %zd bytes.\n", num_read);
  }

  if (num_read == -1) {
    handle_error("read");
  }

  close(sfd);
  exit(EXIT_SUCCESS);
}
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  // TODO: print the message received from client
  // TODO: increase total_message_count per message
  struct client_info *the_client = (struct client_info *)arg;
  int cfd = client->cfd;
  int client_id = client->client_id;
  free(the_client);

  char buf[BUF_SIZE];
  ssize_t num_of_read;

  while ((num_of_read = read(cfd, buf, BUF_SIZE - 1)) > 0) {
    buf[num_of_read] = '\0';

    pthread_mutex_lock(&count_mutex);
    total_message_count++;
    int message_num = total_message_count;
    pthread_mutex_unlock(&count_mutex);

    if (num_of_read > 0 && buf[num_of_read - 1] == '\n') {
      buf[num_of_read - 1] = '\0';
    }

    printf("Msg #  %d; Client ID  %d: %s\n", message_num, client_id, buf);
    fflush(stdout);
  }

  close(cfd);
  printf("Ending thread for client %d\n", client_id);
  fflush(stdout);
  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    // TODO: call handle_client() when launching a new thread, and provide
    // client_info
    int cfd = accept(sfd, NULL, NULL);
    if (cfd == -1) {
      if (errno == EINTR) {
        continue;
      }
      perror("accept");
      continue;
    }

    pthread_mutex_lock(&client_id_mutex);
    int the_id = client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    printf("New client created! ID %d on socket FD %d\n", the_id, cfd);
    fflush(stdout);

    struct client_info *client_info_ptr = malloc(sizeof(struct client_info));
    if (!client_info_ptr) {
      perror("malloc");
      close(cfd);
      continue;
    }

    client_info_ptr->cfd = cfd;
    client_info_ptr->client_id = the_id;

    pthread_t client_thread;
    int create_error = pthread_create(&client_thread, NULL, handle_client, client_info_ptr);
    if (create_error != 0) {
      fprintf(stderr, "pthread_create: %s\n", strerror(create_error));
      free(client_info_ptr);
      close(cfd);
      continue;
    }
    pthread_detach(client_thread);
  }
  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
