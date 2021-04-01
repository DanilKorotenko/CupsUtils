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

#include "cupsutils.hpp"

typedef struct
{
    int number_of_destinations;
    cups_dest_t *destinations;
} CupsDestinationsData;

class CupsUtilsImpl
{
public:
    CupsUtilsImpl();
    ~CupsUtilsImpl();

    std::vector<std::string> getPrintersNames();
    std::string getDeviceURIForPrinterWithName(std::string aPrinterName);

private:
    static int destinationsCallback(CupsDestinationsData *destinations_data,
        unsigned flags, cups_dest_t *dest);

    int getDestinations(cups_ptype_t type, cups_ptype_t mask,
        CupsDestinationsData *aDestinationsData);

    void freeDestinationsData(CupsDestinationsData *aDestinationsData);

    void updateDestinationsData();

    const char *getDeviceURIForDestination(cups_dest_t *destination);

    CupsDestinationsData _destinations_data;
};

#endif /* cupsutilsimpl_hpp */
