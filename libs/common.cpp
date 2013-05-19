



#include "common.h"
#include <iostream>

Sigfunc *signal (int signo, Sigfunc *func) {

    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef  SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;     /* SunOS 4.x */
#endif
    } else {
#ifdef  SA_RESTART
        act.sa_flags |= SA_RESTART; /* SVR4, 4.4BSD */
#endif
    }
    if (sigaction (signo, &act, &oact) < 0)
        return (SIG_ERR);
    return (oact.sa_handler);
}



void Write(int iFD, char* buf, ssize_t size) {
   
    ssize_t writed_len;
    char* p = buf;
    while(size) {
        std::cout << "trying to write " << size << " to client" << std::endl;
        writed_len = write(iFD, (void*)p, size);
        if((writed_len == -1) && (errno==EINTR)) {
            std::cout << "interrupt by signal" << std::endl;
            continue;
        } else if(writed_len == -1) {
            std::cout << "write system error" << std::endl;
            exit(-1);
        } 
        std::cout << "writed " << writed_len << " to client" << std::endl;
        if(!(size-writed_len)) {
            std::cout << "finish write data" << std::endl;
            return;
        } else {
            size -= writed_len;
            p += writed_len;
        }
        
    }
}



void ReadLine(int iFD, void* line_buf, size_t size) {
    
    ssize_t len;
    char* p = (char*)line_buf;
    while(1) {
        len = read(iFD, p, size);
        if(len == -1) {
            if(errno == EINTR) {
                std::cout << "interrupt by signal" << std::endl;
                continue;
            } else {
                std::cout << "read system error" << std::endl;
                exit(-1);
            }
        } else if(len == 0) {
            std::cout << "read eof" << std::endl;
            exit(0);
            return;
        } else {
            std::cout << "read data " << len << " bytes" << std::endl; 
            if(p[len-1] == '\n') {
                std::cout << "finish read data" << std::endl;
                return;
            } else {
                p += len;
                size -= len;
                if(size <= 0) {
                    std::cout << "recv data bigger than buf" << std::endl;
                    exit(-1);
                }
            }
        }
    }
}
