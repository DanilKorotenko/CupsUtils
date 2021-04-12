//
//  cupsutilsimpl.hpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#ifndef cupsutilsimpl_hpp
#define cupsutilsimpl_hpp

#include <memory>
#include <cups/cups.h>

#include "CupsUtils.hpp"

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

    std::vector<std::string> getPrintersNames();
    std::vector<CupsOption> getOptionsForPrinterWithName(std::string aPrinterName);
    std::string getOptionValueForPrinterWithName(std::string aPrinterName, std::string anOptionName);
    bool setOptionForPrinterWithName(std::string aPrinterName,
        const CupsOption &anOption);
    bool checkURI(std::string anUri);
    bool getDocument(
        const std::string &aPrinterURI,
        const std::string &aJobIDStr,
        const std::string &aDocumentNumberStr,
        const std::string &anOutputFileName);
    bool setPrinterHoldNewJobs(
        const std::string &aPrinterName);

#pragma mark Private
private:
    static int destinationsCallback(CupsDestinationsData *destinations_data,
        unsigned flags, cups_dest_t *dest);
    int getDestinations(cups_ptype_t type, cups_ptype_t mask,
        CupsDestinationsData *aDestinationsData);
    void freeDestinationsData(CupsDestinationsData *aDestinationsData);
    void updateDestinationsData();
    cups_dest_t *getPrinterDestinationWithName(std::string aPrinterName);
    std::string getPrinterURIWithName(std::string aPrinterName);

    CupsDestinationsData _destinations_data;
};

#endif /* cupsutilsimpl_hpp */
