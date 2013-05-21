
/*
 *
 *
 *
 */

#include "echo.h"
#include <iostream>

void echo_client(int iFD) {
    
    char sendline[MAX_LINE],recvline[MAX_LINE];
    fd_set read_fds;
    int maxfd;
    int stdfd = fileno(stdin);
    FD_ZERO(&read_fds);
    int selectd;
    int index=1;
    while(1) {
        bzero(sendline, MAX_LINE);
        bzero(recvline, MAX_LINE);
        FD_SET(iFD, &read_fds);  
        if(index){
            FD_SET(stdfd, &read_fds); 
            maxfd = max(iFD, stdfd) + 1; 
        } else {
            maxfd = iFD + 1;
        }
        selectd = select(maxfd, &read_fds, NULL, NULL, NULL);
        for(int i=0;i<selectd;i++) {
            std::cout << "number : " << selectd << std::endl;
            if( FD_ISSET(iFD, &read_fds)){
                std::cout << "comming ....." << std::endl;
                ReadLine(iFD, recvline, MAX_LINE);   
                puts(recvline);
            }
            if(FD_ISSET(stdfd, &read_fds)){
                if(gets(sendline) == NULL) {
                    if(ferror(stdin)) {
                        std::cout << "gets error" << std::endl;
                        return;
                    } else {
                        std::cout << "gets eof, exit" << std::endl;
                        return;
                    }
                } else {
                    std::cout << "Read cmd : " << sendline << std::endl;
                    //add rt
                    sendline[strlen(sendline)] = '\n';
                    Write(iFD, sendline, strlen(sendline));
                }
            }
        }
    }
}

