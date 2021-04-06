//
//  PPDUtilsImpl.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#ifndef PPDUtilsImpl_hpp
#define PPDUtilsImpl_hpp

#include <memory>
#include <cups/cups.h>

#include "PPDUtils.hpp"

class PPDUtilsImpl
{
#pragma mark Public
public:
    PPDUtilsImpl();
    ~PPDUtilsImpl();

    std::string getPPDFilePathForPrinterName(const std::string &aPrinterName);

#pragma mark Private
private:

};

#endif /* PPDUtilsImpl_hpp */
