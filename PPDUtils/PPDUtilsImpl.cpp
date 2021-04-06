//
//  PPDUtilsImpl.cpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#include "PPDUtilsImpl.hpp"
#include <iostream>
#include <cups/ppd.h>

//#include "ppdc/ppdc.h"

const char *kPPDFilesPath = "/etc/cups/ppd";

PPDUtilsImpl::PPDUtilsImpl()
{
}

PPDUtilsImpl::~PPDUtilsImpl()
{
}

#pragma mark Public
////////////////////////////////////////////////////////////////////////////////
// Public

std::string PPDUtilsImpl::getPPDFilePathForPrinterName(
    const std::string &aPrinterName)
{
    std::string ppdFilePath2 = kPPDFilesPath;
    ppdFilePath2 += "/";
    ppdFilePath2 += aPrinterName;
    ppdFilePath2 += ".ppd";

    return ppdFilePath2;

    const char *ppdFilePath = cupsGetPPD(aPrinterName.c_str());
//    const char *ppdFilePath = cupsGetServerPPD(CUPS_HTTP_DEFAULT, aPrinterName.c_str());
    std::string result(ppdFilePath);
    return result;
}

#pragma mark Private
////////////////////////////////////////////////////////////////////////////////
// Private


