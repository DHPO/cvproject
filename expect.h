#ifndef __EXPECT__
#define __EXPECT__
#include <string>
#include <iostream>
#define ABORT true

void expect(bool expr, std::string errMsg)
{
    if (!expr) {
        if (ABORT)
            throw(errMsg);
        else
            std::cerr << "[Error] " << errMsg << std::endl;
    }
}

#endif