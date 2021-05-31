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
#include "CupsJob.hpp"

namespace CupsUtilities
{

class CupsUtilsImpl;

extern const char *kOptionNameDeviceURI;
extern const char *kOptionNamePrinterURI;
extern const char *kOptionNamePrinterOperationPolicy;
extern const char *kOptionNameJobHoldUntilDefault;
extern const char *kJobHoldUntilDefaultOptionValueIndefinite;

extern const char *kOptionNamePrinterState;
extern const char *kOptionNamePrinterStateReasons;
extern const char *kOptionNamePrinterInfo;

class CupsUtils
{
public:
    CupsUtils();
    ~CupsUtils();

    static int getJobNumberOfDocuments(int aJobID);
    static bool getDocument(int aJobID, int aDocumentNumber,
        const std::string &anOutputFileName);
    static void cancelJob(int aJobId);
    static bool releaseJob(int aJobId);
    static std::string lastErrorString();

    std::vector<CupsPrinter> getPrinters();
    std::vector<CupsOption> getListOfOptionsForPrinterWithName(std::string aPrinterName);
    std::string getOptionValueForPrinterWithName(std::string aPrinterName, std::string anOptionName);
    bool setOptionForPrinterWithName(std::string aPrinterName, const CupsOption &anOption);
    bool checkURI(std::string anUri);
    std::vector<CupsJob::PtrT> getActiveJobs();

private:
    std::unique_ptr<CupsUtilsImpl> _impl;
};

}

#endif /* cupsutils_hpp */
