#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <signal.h>
#include <pthread.h>


int continuer = 1;

char* nomSignaux [] = {
                        "SIGHUP",     // 01
                        "SIGINT",     // 02
                        "SIGQUIT",    // 03
                        "SIGILL",     // 04
                        "SIGTRAP",    // 05
                        "SIGABRT",    // 06
                        "SIGBUS",     // 07
                        "SIGFPE",     // 08
                        "SIGKILL",    // 09
                        "SIGUSR1",    // 10
                        "SIGSEGV",    // 11
                        "SIGUSR2",    // 12
                        "SIGPIPE",    // 13
                        "SIGALRM",    // 14
                        "SIGTERM",    // 15
                        "SIGSTKFLT",  // 16
                        "SIGCHLD",    // 17
                        "SIGCONT",    // 18
                        "SIGSTOP",    // 19
                        "SIGTSTP",    // 20
                        "SIGTTIN",    // 21
                        "SIGTTOU",    // 22
                        "SIGURG",     // 23
                        "SIGXCPU",    // 24
                        "SIGXFSZ",    // 25
                        "SIGVTALRM",  // 26
                        "SIGPROF",    // 27
                        "SIGWINCH",   // 28
                        "SIGPOLL",    // 29
                        "SIGPWR",     // 30
                        "SIGSYS",     // 31
                        "SIGUNUSED"   // 32
                      };

typedef struct
 {
  pthread_t thread;
  int       sockfd;
  int       id;
  int       actif;
 }
 GestionnaireConnexion;

GestionnaireConnexion tab [10];

void traiterSignal (int signalRecu, siginfo_t* info, void* pasUtileIci)
 {
  printf ("\nOn a reçu le signal %s, on arrête le serveur\n", nomSignaux [signalRecu] );
  continuer = 0;
 }

void* traiterConnexionEntrante ( void* pt )
 {
  char* tampon;
  uint16_t nbOctets;

  GestionnaireConnexion* gestionnaire = (GestionnaireConnexion*) pt;

  nbOctets = 0;

  read (gestionnaire->sockfd, &nbOctets, sizeof (int16_t));
  printf ("Serveur (Thread %d) reçoit %u octets\n", gestionnaire->id, nbOctets);

  tampon = (char*) malloc ( nbOctets + 1);

  if (tampon)
   {
    bzero ((char *) tampon, nbOctets + 1);
 
    read (gestionnaire->sockfd, tampon, nbOctets);
    printf ("Serveur (Thread %d) reçoit : %s\n", gestionnaire->id, tampon);
 
    free (tampon);
   }
 
  char messageRenvoye [] = "Message reçu par le thread x";
 
  sprintf (messageRenvoye, "Message reçu par le thread %d", gestionnaire->id);

  nbOctets = strlen (messageRenvoye);

  write (gestionnaire->sockfd, &nbOctets, sizeof (int16_t));
  write (gestionnaire->sockfd, messageRenvoye, nbOctets);
 
  printf ("Serveur (Thread %d) envoie :%s\n", gestionnaire->id, messageRenvoye);

  close (gestionnaire->sockfd);

  sleep (2);

  gestionnaire->actif = 0;

  printf ("Serveur (Thread %d) ferme la connexion et se termine\n", gestionnaire->id);

  pthread_exit (NULL);
 }

int main (int argc, char** argv)
 {
  if ( argc != 2 )
   {
    printf ("Usage : serveurUnix_4 nomDuFichier\n");
    exit (-1);
   }

  struct sigaction prepaSignal;
                   prepaSignal.sa_sigaction=&traiterSignal;
                   prepaSignal.sa_flags=SA_SIGINFO;// | SA_RESTART;  // Pour redémarrer automatiquement le accept => ça empêcherait un arrêt immédiat (il faudrait traiter 1 connexion)

  sigemptyset (&prepaSignal.sa_mask);

  sigaction (SIGINT, &prepaSignal, NULL);

  int sockfd, newsockfd;
  socklen_t clilen, servlen;

  struct sockaddr_un cli_addr;
  struct sockaddr_un serv_addr;

  for (int i=0; i<10; i++)
   {
    tab [i].actif = 0;
    tab [i].id    = i+1;
   }
 

  if ( (sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
   {
    printf ("Erreur de creation de socket\n");  
    exit (-2);
   }

  printf ("Création de la structure sockaddr_un qui utilise le fichier %s\n", argv[1]);

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sun_family = AF_UNIX;
  strcpy(serv_addr.sun_path, argv[1]);
  servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);



  if ( bind (sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
   {
    printf ("Erreur de bind\n");    
    exit (-3);
   }

  listen(sockfd, 5);

  while ( continuer )
   {
    clilen = sizeof(cli_addr);

    printf ("serveur (Thread principal) : En attente...\n");

    newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0)
          {
           printf ("serveur (Thread principal) : Erreur de accept\n");
           continuer = 0;
          }
     else {
           int num=0;

           while (tab[num].actif)
            {
             printf ("Le thread numero %d est occupe\n", (num+1));
             num = (num+1) % 10;
            }

           printf ("On sélectionne le thread %d\n", (num+1));

           tab[num].actif = 1;
           tab[num].sockfd = newsockfd;

           pthread_create ( &(tab[num].thread), NULL, traiterConnexionEntrante, tab+num );
          }
   }

  close (sockfd);

  printf ("Serveur (Thread principal) ferme la connexion principale et s'arrête\n");

  unlink (argv[1]);

  return 0;
 }

