//
//  cupsutilsimpl.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#pragma once

#include <memory>
#include <cups/cups.h>

#include "CupsUtils.hpp"

namespace CupsUtilities
{

extern const char* kOptionNameDeviceURI;
extern const char* kOptionNamePrinterURI;
extern const char* kOptionNamePrinterOperationPolicy;

extern const char* kOptionNameJobHoldUntilDefault;
extern const char* kJobHoldUntilDefaultValueIndefinite;
extern const char* kJobHoldUntilDefaultValueNoHold;

extern const char* kOptionNamePrinterState;
extern const char* kOptionNamePrinterStateReasons;
extern const char* kOptionNamePrinterInfo;

typedef struct
{
    int number_of_destinations;
    cups_dest_t *destinations;
} CupsDestinationsData;

class CupsUtilsImpl
{
#pragma mark Public
public:
    CupsUtilsImpl();
    ~CupsUtilsImpl();

    static int getJobNumberOfDocuments(int aJobID);
    static bool getDocument(int aJobID, int aDocumentNumber,
        const std::string &anOutputFileName);
    static void cancelJob(int aJobId);
    static bool releaseJob(int aJobId);
    static std::string lastErrorString();

    std::vector<CupsPrinter> getPrinters();
    CupsPrinter getPrinterWithName(std::string aPrinterName);
    CupsPrinter getPrinterWithDestination(cups_dest_t *printerDestination);

    std::vector<CupsOption> getOptionsForPrinterWithName(std::string aPrinterName);

    std::string getOptionValueForPrinterWithName(
        cups_dest_t *aPrinterDestination, std::string anOptionName);
    std::string getOptionValueForPrinterWithName(
        std::string aPrinterName, std::string anOptionName);
    bool setOptionForPrinterWithName(std::string aPrinterName,
        const CupsOption &anOption);
    bool checkURI(std::string anUri);
    std::vector<CupsJob::PtrT> getActiveJobs();

#pragma mark Private
private:
    static int destinationsCallback(CupsDestinationsData *destinations_data,
        unsigned flags, cups_dest_t *dest);
    int getDestinations(cups_ptype_t type, cups_ptype_t mask,
        CupsDestinationsData *aDestinationsData);
    void freeDestinationsData(CupsDestinationsData *aDestinationsData);
    bool setPrinterOptions(
        int num_options,
        cups_option_t *options,
        const char *printerUri);

    CupsDestinationsData _destinations_data = { 0, NULL };

};

} // namespace CupsUtilities
