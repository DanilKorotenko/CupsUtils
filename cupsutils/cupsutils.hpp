//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#ifndef cupsutils_hpp
#define cupsutils_hpp

#include <string>
#include <vector>

class CupsUtilsImpl;

class CupsUtils
{
public:
    CupsUtils();
    ~CupsUtils();

    std::vector<std::string> getPrintersNames();

    std::string getDeviceURIForPrinterWithName(std::string aPrinterName);

private:
    std::unique_ptr<CupsUtilsImpl> _impl;
};

#endif /* cupsutils_hpp */
