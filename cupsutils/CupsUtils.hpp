//
//  cupsutils.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#pragma once

#include <string>
#include <vector>
#include <map>
#include "CupsUtilsTypes.hpp"

namespace CupsUtilities
{

class CupsUtilsImpl;

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

} // namespace CupsUtilities
