#include "common.h"
#include <iostream>
#include "echo.h"
#define SA struct sockaddr*
#define CSA const SA

void sig_pipe(int signo)
{
    printf("%s","pipe break\n");
    return;
}


int main(int argc, char** argv) {

    int iFD;
    iFD = socket(AF_INET, SOCK_STREAM, 0);
    if(iFD == -1) {
        perror("socket");
        goto failed;
    }

    struct sockaddr_in addr;
    bzero((void*)&addr,sizeof(addr));
    addr.sin_port = htons((in_port_t)(atoi(argv[2])));
    addr.sin_family = AF_INET;  
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    std::cout<< "Server IP : " << argv[1] << ", Port : " << argv[2] << std::endl;

    signal(SIGPIPE,sig_pipe);

    //connect
    if(connect(iFD, (CSA)&addr, sizeof(addr))) {
        perror("connect");
        goto failed;
    }

    echo_client(iFD);
    return 0; 
     
failed:
    exit(-1);
}
