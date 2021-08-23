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

    // Cups supported few documents in one job. We need number of the jobs documents.
    // @param jobID - CUPS job id.
    // @return number of the jobs documents.
    static int getJobNumberOfDocuments(int aJobID);
    //Save CUPS job document to file.
    // @param jobID - CUPS job id.
    //@param documentId - Document id for this job. IDs starts from 1.
    // @param outputFileName - path to save.
    // @return true - if operation successed.
    static bool getDocument(int aJobID, int aDocumentNumber,
        const std::string &anOutputFileName);
    // Cancel job.
    // @param jobId - CUPS job id.
    static void cancelJob(int aJobId);
    // Release job to start print documents.
    // @param jobId - CUPS job id.
    // @return true - if operation successed.
    static bool releaseJob(int aJobId);
    static std::string lastErrorString();

    // Return known printers.
    // @return List of printers.
    std::vector<CupsPrinter> getPrinters();

    // Get list of printer options.
    // @param printerName - CUPS printer name.
    // @return List of printer options.
    std::vector<CupsOption> getListOfOptionsForPrinterWithName(std::string aPrinterName);

    // Load few properties from CUPS job history file.
    // @param jobHistFile - job history file path.
    // @return true - if operation successed.
    // @throw std::runtime_error.
    static std::vector<CupsOption> loadJobProperties(const std::string& jobHistFile);

    // Get requested printer option.
    // @param printerName - CUPS printer name.
    // @param optionName - option name.
    // @return empty string if fails.
    std::string getOptionValueForPrinterWithName(std::string aPrinterName, std::string anOptionName);

    // Set requested printer option.
    // @param printerName - CUPS printer name.
    // @param option - option struct.
    // @return true - if operation successed.
    bool setOptionForPrinterWithName(std::string aPrinterName, const CupsOption &anOption);


    // Tries ot separate a URI into its components.
    // @param uri
    // @return true - if operation successed.
    bool checkURI(std::string anUri);

    // Get list of active CUPS jobs.
    // @return list of active CUPS jobs.
    std::vector<CupsJob::PtrT> getActiveJobs();

    //// Set DLP specific options /////


    // Set option to hold job until we release one.
    // @param printer - cups printer
    // @return true - if operation successed.
    bool setJobHoldUntilDefaultIndefinite(const CupsPrinter& printer);


    // Unset option to hold job to default.
    // @param printer - cups printer
    // @return true - if operation successed.
    bool unsetJobHoldUntilDefaultIndefinite(const CupsPrinter& printer);


    // Set policy with permissions for printer.
    // @param printer - cups printer
    // @param policyName - Policy name have to be injected to /etc/cups/cupsd.conf
    // @return true - if operation successed.
    bool setPrinterOperationPolicy(const CupsPrinter& printer, const std::string& policyName);

private:
    std::unique_ptr<CupsUtilsImpl> _impl;
};

} // namespace CupsUtilities
