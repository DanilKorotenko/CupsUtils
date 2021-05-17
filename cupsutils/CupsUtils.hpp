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
#include "CupsUtilsTypes.hpp"

namespace CupsUtilities
{

class CupsUtilsImpl;

extern const char *kDeviceURIOptionName;
extern const char *kPrinterURIOptionName;
extern const char *kPrinterOperationPolicyOptionName;
extern const char *kJobHoldUntilDefaultOptionName;
extern const char *kJobHoldUntilDefaultOptionValueIndefinite;

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
    int getJobNumberOfDocuments(int aJobID);
    bool getDocument(int aJobID, int aDocumentNumber,
        const std::string &anOutputFileName);
    std::vector<CupsJob> getActiveJobs();
    void cancelJob(int aJobId);
    void cancelAllJobs();
    bool releaseJob(int aJobId);

private:
    std::unique_ptr<CupsUtilsImpl> _impl;
};

}

#endif /* cupsutils_hpp */
