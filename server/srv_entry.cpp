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
#include <sys/select.h>
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
    

    //add to
    std::vector<int> vecFD;
    std::vector<int>::size_type size = 0; 

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

    //setup SIG_CHILD
    signal(SIGCHLD, sig_chld);   
    std::cout << "Being to accept, SOCK LISTENED....." << std::endl;
    //accept
    socklen_t len;
    pid_t iChild;
    int count;
    fd_set set, allset;
    FD_ZERO(&set);
    FD_ZERO(&allset);
    FD_SET(iListenFD, &allset); 
    int maxd = 0;
    char buf[MAX_LINE];
    int ilen;
    maxd = max(maxd, iListenFD) + 1;   
    while(1) {
        set = allset; 
        count = select(maxd, &set, NULL, NULL, NULL);
        if(FD_ISSET(iListenFD, &set)) {
            bzero((void*)&peer_addr,sizeof(peer_addr));
            len = sizeof(peer_addr);
            iChildFD = accept(iListenFD, (SA)&peer_addr, &len);
            if(iChildFD == -1) {
                perror("accept");
                exit(0);
            } else {
                std::cout << "new one :" << iChildFD << std::endl;
                FD_SET(iChildFD, &allset);
                maxd = max(maxd, iChildFD) + 1;
                vecFD.push_back(iChildFD);
                if(!--count){
                    continue;
                }
            }
        }
        std::vector<int>::iterator ip = vecFD.begin();
        for(; ip != vecFD.end(); ip++) {
            if(FD_ISSET(*ip, &set)){
               //read
               count--;
               bzero(buf,MAX_LINE);
               ilen = read(*ip, buf, MAX_LINE);
               if(ilen){
                   std::cout << "Data size : " << ilen << ", data : " << buf << std::endl;
                   Write(*ip, buf, ilen);
               } else {
                   std::cout << "resv eof, so i exit" << std::endl;
                   ip = vecFD.erase(ip);  
                   FD_CLR(*ip, &set);
                   close(*ip);
               }
            }
            if(count==0){
                break;
            }
        }
        std::cout << "client count : " << vecFD.size() << std::endl;
    } 
    return 0;
}
