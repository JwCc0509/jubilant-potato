#pragma once
#include <stdexcept>
#include "GenTL.h"
#include <string>
#include "windows.h"
using namespace std;

/*
try
{
    ...
}
catch (const gentl_error &ge) 
{
    printf("gentl error is: %d\n", ge.scap_err);    
}
*/
class scap_gentl_error : public std::runtime_error {
public:
    GC_ERROR scap_err;
    explicit scap_gentl_error(GC_ERROR err)
        : std::runtime_error(std::string("GenTL error ") + std::to_string((uint64_t)err))
        , scap_err(err)
    {}
    scap_gentl_error(GC_ERROR err, const std::string& description)
        : std::runtime_error(std::string("GenTL error ") + std::to_string((uint64_t)err) + description)
        , scap_err(err)
    {}
};
