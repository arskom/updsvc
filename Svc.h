#ifndef SVC_H
#define SVC_H

#include <sstream>
#include "json.hpp"

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
std::string GetProgramVersion();
std::string UpdateDetector(std::string sstr);


#endif // SVC_H
