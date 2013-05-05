
/*
 *
 *
 *
 */

#include "echo.h"

void echo_server(int iFD) {
    std::cout<< getpid() << " >> exit" << std::endl;

    char buf[MAX_LINE];
    ssize_t len;
    while(1) {
        bzero(buf,MAX_LINE);
        len = read(iFD, buf, MAX_LINE);
        if((len == -1) && (errno == EINTR)){
            std::cout << "interrupt by signal, read again." << std::endl;
            continue;
        } else if (len == -1) {
            std::cout << "interrupt by error." << std::endl;
            return;
        } else {
            std::cout << "Data size : " << len << ", data : " << buf << std::endl;
            Write(iFD, buf, len);
        }
    }
}



