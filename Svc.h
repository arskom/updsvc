#ifndef SVC_H
#define SVC_H

#include <sstream>

struct Buffer{
    char* data;
    unsigned long size;

    Buffer():data(nullptr),size(0) {}
    Buffer(char *d,unsigned long s):data(d),size(s) {}

    ~Buffer(){
        if(data){
            delete [] data;
        }

    }

};

std::string CreateRequest();

#endif // SVC_H
