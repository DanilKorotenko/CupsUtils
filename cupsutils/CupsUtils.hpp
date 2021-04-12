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

extern const char *kDeviceURIOptionName;
extern const char *kPrinterURIOptionName;

class CupsUtils
{
public:
    CupsUtils();
    ~CupsUtils();

    std::vector<std::string> getPrintersNames();
    std::vector<CupsOption> getListOfOptionsForPrinterWithName(std::string aPrinterName);
    std::string getOptionValueForPrinterWithName(std::string aPrinterName, std::string anOptionName);
    bool setOptionForPrinterWithName(std::string aPrinterName, const CupsOption &anOption);
    bool checkURI(std::string anUri);
    bool getDocument(
        const std::string &aPrinterURI,
        const std::string &aJobIDStr,
        const std::string &aDocumentNumberStr,
        const std::string &anOutputFileName);
    bool setPrinterHoldNewJobs(
        const std::string &aPrinterName);

private:
    std::unique_ptr<CupsUtilsImpl> _impl;
};

#endif /* cupsutils_hpp */
