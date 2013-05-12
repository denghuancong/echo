/*
 *
 *
 *
 *
 *
 *
 *
 */

#include "srv_entry.h"

void sig_chld(int signo)
{
    pid_t    pid;
    int      stat;

    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

int main(int argc, char** argv) {
    
    int iListenFD, iChildFD;
    iListenFD = socket(AF_INET, SOCK_STREAM, 0);
    if(iListenFD == -1) {
        perror("socket");
        goto failed;
    }

    struct sockaddr_in listen_addr, peer_addr;
    bzero((void*)&listen_addr,sizeof(listen_addr));
    listen_addr.sin_port = htons((in_port_t)atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &listen_addr.sin_addr);
    listen_addr.sin_family = AF_INET;  
    //bind
    std::cout<< "Bind IP : " << argv[1] << ", Port : " << argv[2] << std::endl;
    if(bind(iListenFD, (CSA)&listen_addr, sizeof(listen_addr))) {
        perror("bind");
        goto failed;
    }
    
    //listen
    if(listen(iListenFD, BACK_LOG)) {
        perror("listen");
        goto failed;
    }

    //setup SIG_CHILD
    signal(SIGCHLD, sig_chld);   

    std::cout << "Being to accept, SOCK LISTENED....." << std::endl;
   //accept
   socklen_t len;
   pid_t iChild;
   while(1) {
       bzero((void*)&peer_addr,len);
       len = sizeof(peer_addr);
       iChildFD = accept(iListenFD, (SA)&peer_addr, &len);
       if(errno == EINTR){
           continue;
       }
       if(iChildFD == -1) {
           perror("accept");
           goto failed;
       } else {
           iChild = fork();
           if(iChild) { //parent
               std::cout << "New Child , pid is " << iChild << std::endl;
               //release socket fd of child
               close(iChildFD);
           } else {
               //release socket fd of father
               close(iListenFD);
               echo_server(iChildFD);
               //never execute following code
               goto failed;
           }
       }
   } 
   return 0;

failed:
    exit(-1);
}
