//
//  cupsutils.cpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#include "cupsutils.hpp"
#include "cupsutilsimpl.hpp"


CupsUtils::CupsUtils()
    : _impl(new CupsUtilsImpl())
{

}

CupsUtils::~CupsUtils()
{
    _impl.reset();
}

std::vector<std::string> CupsUtils::getPrintersNames()
{
    if (_impl)
    {
        return _impl->getPrintersNames();
    }
    std::vector<std::string> result;
    return result;
}
