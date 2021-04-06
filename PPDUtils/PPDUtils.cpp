//
//  PPDUtils.cpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#include "PPDUtils.hpp"
#include "PPDUtilsImpl.hpp"

PPDUtils::PPDUtils()
    : _impl(new PPDUtilsImpl())
{
}

PPDUtils::~PPDUtils()
{
    _impl.reset();
}

std::string PPDUtils::getPPDFilePathForPrinterName(const std::string &aPrinterName)
{
    if (_impl)
    {
        return _impl->getPPDFilePathForPrinterName(aPrinterName);
    }
    std::string result;
    return result;
}
