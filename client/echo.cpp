
/*
 *
 *
 *
 */

#include "echo.h"
#include <iostream>

void echo_client(int iFD) {
    
    char sendline[MAX_LINE],recvline[MAX_LINE];
    while(1) {
        bzero(sendline, MAX_LINE);
        bzero(recvline, MAX_LINE);
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
            ReadLine(iFD, recvline, MAX_LINE);   
            puts(recvline);
        }
    }
}

