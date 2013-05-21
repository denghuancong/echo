/*
 *
 *
 *
 *
 *
 *
 *
 */

#include <vector>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#define OPEN_MAX  3
#include <poll.h>
#include "srv_entry.h"
int findAvailFD(struct pollfd* pFD)
{
    for(int i=1;i<OPEN_MAX;i++){
        if(pFD[i].fd == -1 ) return i;
    }
    return -1;
}
void sig_chld(int signo)
{
    pid_t    pid;
    int      stat;

    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return;
}

int main(int argc, char** argv) {
    
    struct pollfd fdarr[OPEN_MAX];
    int iListenFD, iChildFD;
    iListenFD = socket(AF_INET, SOCK_STREAM, 0);
    if(iListenFD == -1) {
        perror("socket");
        exit(-1);
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
        exit(-1);
    }
    
    //listen
    if(listen(iListenFD, BACK_LOG)) {
        perror("listen");
        exit(-1);
    }
    fdarr[0].fd = iListenFD;
    fdarr[0].events = POLLIN; 
    for(int i=1;i<OPEN_MAX;i++){
        fdarr[i].fd = -1;
    }
    int size = 1;

    //setup SIG_CHILD
    signal(SIGCHLD, sig_chld);   
    std::cout << "Being to accept, SOCK LISTENED....." << std::endl;
    //accept
    socklen_t len;
    pid_t iChild;
    int ready; 
    int ilen;
    char buf[MAX_LINE];
    while(1) {
        ready = poll(fdarr, size + 1, -1);
        if(fdarr[0].revents & POLLIN) {
            bzero((void*)&peer_addr,sizeof(peer_addr));
            len = sizeof(peer_addr);
            iChildFD = accept(iListenFD, (SA)&peer_addr, &len);
            if(iChildFD == -1) {
                perror("accept");
                exit(0);
            } else {
                std::cout << "new one :" << iChildFD << std::endl;
                int j = findAvailFD(fdarr);
                if(j==-1){
                    std::cout << "Max client" << std::endl; 
                    exit(0);
                }
                fdarr[j].fd = iChildFD;
                fdarr[j].events = POLLIN;
                size ++;
                if(!--ready){
                    continue;
                }
            }
        }
        for(int i=1;i<OPEN_MAX;i++) {
            if(fdarr[i].revents & POLLIN){
               //read
               ready--;
               bzero(buf,MAX_LINE);
               ilen = read(fdarr[i].fd, buf, MAX_LINE);
               if(ilen < 0){
                   std::cout << "reset" << std::endl;
                   close(fdarr[i].fd);
                   fdarr[i].fd = -1;
                   size --;
               } else if(ilen>0){
                   std::cout << "Data size : " << ilen << ", data : " << buf << std::endl;
                   Write(fdarr[i].fd, buf, ilen);
               } else {
                   std::cout << "resv eof, so i exit" << std::endl;
                   close(fdarr[i].fd);
                   fdarr[i].fd = -1;
                   size --;
               }
            }
            if(ready==0){
                break;
            }
        }
        std::cout << "client count : " << size << std::endl;
    } 
    return 0;
}

