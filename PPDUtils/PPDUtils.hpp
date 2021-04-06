//
//  PPDUtils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#ifndef PPDUtils_hpp
#define PPDUtils_hpp

#include <string>
#include <vector>

class PPDUtilsImpl;

class PPDUtils
{
public:
    PPDUtils();
    ~PPDUtils();

    std::string getPPDFilePathForPrinterName(const std::string &aPrinterName);

private:
    std::unique_ptr<PPDUtilsImpl> _impl;
};

#endif /* PPDUtils_hpp */
