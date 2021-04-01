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

typedef struct
{
    std::string name;
    std::string value;
} CupsOption;

class CupsUtils
{
public:
    CupsUtils();
    ~CupsUtils();

    std::vector<std::string> getPrintersNames();

    std::string getDeviceURIForPrinterWithName(std::string aPrinterName);

    std::vector<CupsOption> getOptionsForPrinterWithName(std::string aPrinterName);

private:
    std::unique_ptr<CupsUtilsImpl> _impl;
};

#endif /* cupsutils_hpp */
