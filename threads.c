#include <pthread.h>

//Thread serveur
typedef struct
 {
  pthread_t thread;
  int       sockfd;
 }
 ThreadServeur;

//Thread client
typedef struct
 {
  pthread_t thread;
  int       sockfd;
 }
 ThreadCLient;

//Thread de calcul
typedef struct
 {
  pthread_t thread;
 }
 ThreadCalcul;

//Thread trace
typedef struct
 {
  pthread_t thread;
 }
 ThreadTrace;