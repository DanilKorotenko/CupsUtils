//
//  cupsutilsimpl.cpp
//  cupsutils
//
//  Created by Danil Korotenko on 4/1/21.
//

#include "cupsutilsimpl.hpp"

CupsUtilsImpl::CupsUtilsImpl()
{
    _destinations_data = { 0, NULL };
    updateDestinationsData();
}

CupsUtilsImpl::~CupsUtilsImpl()
{
    freeDestinationsData(&_destinations_data);
}

#pragma mark Public
////////////////////////////////////////////////////////////////////////////////
// Public

std::vector<std::string> CupsUtilsImpl::getPrintersNames()
{
    std::vector<std::string> result;

    for (int i = 0; i < _destinations_data.number_of_destinations; i++)
    {
        result.push_back(_destinations_data.destinations[i].name);
    }

    return result;
}

std::string CupsUtilsImpl::getDeviceURIForPrinterWithName(std::string aPrinterName)
{
    cups_dest_t *printerDestination = getPrinterDestinationWithName(aPrinterName);

    std::string result(this->getDeviceURIForDestination(printerDestination));

    return result;
}

std::vector<CupsOption> CupsUtilsImpl::getOptionsForPrinterWithName(std::string aPrinterName)
{
    std::vector<CupsOption> result;

    cups_dest_t *printerDestination = getPrinterDestinationWithName(aPrinterName);

    for (int i = 0; i < printerDestination->num_options; i++)
    {
        cups_option_t option = printerDestination->options[i];

        CupsOption cupsOption = { option.name, option.value };

        result.push_back(cupsOption);
    }

    return result;
}

#pragma mark Private
////////////////////////////////////////////////////////////////////////////////
// Private

int CupsUtilsImpl::destinationsCallback(CupsDestinationsData *destinations_data,
    unsigned flags, cups_dest_t *dest)
{
    if (flags & CUPS_DEST_FLAGS_REMOVED)
    {
        // Remove destination from array...

        destinations_data->number_of_destinations =
            cupsRemoveDest(dest->name, dest->instance,
                destinations_data->number_of_destinations,
                &(destinations_data->destinations));
    }
    else
    {
        // Add destination to array...

        destinations_data->number_of_destinations =
            cupsCopyDest(dest, destinations_data->number_of_destinations,
                     &(destinations_data->destinations));
    }

    return (1);
}

int CupsUtilsImpl::getDestinations(cups_ptype_t type, cups_ptype_t mask,
    CupsDestinationsData *aDestinationsData)
{
    if (!cupsEnumDests(CUPS_DEST_FLAGS_NONE, 5000, NULL, type,
        mask, (cups_dest_cb_t)CupsUtilsImpl::destinationsCallback,
        aDestinationsData))
    {
        // An error occurred, free all of the destinations and
        // return...

        cupsFreeDests(aDestinationsData->number_of_destinations, aDestinationsData->destinations);

        aDestinationsData->number_of_destinations = 0;
        aDestinationsData->destinations = NULL;

        return (0);
    }

    // Return the destination array...

    return (aDestinationsData->number_of_destinations);
}

void CupsUtilsImpl::freeDestinationsData(CupsDestinationsData *aDestinationsData)
{
    cupsFreeDests(aDestinationsData->number_of_destinations,
        aDestinationsData->destinations);

    *aDestinationsData = {0, NULL};
}

void CupsUtilsImpl::updateDestinationsData()
{
    getDestinations(CUPS_PRINTER_LOCAL, CUPS_PRINTER_DISCOVERED, &_destinations_data);
}

cups_dest_t *CupsUtilsImpl::getPrinterDestinationWithName(std::string aPrinterName)
{
    cups_dest_t *printerDestination = cupsGetDest(aPrinterName.c_str(), NULL,
        _destinations_data.number_of_destinations, _destinations_data.destinations);

    return printerDestination;
}

const char *CupsUtilsImpl::getDeviceURIForDestination(cups_dest_t *destination)
{
//    const char *result = cupsGetOption("printer-uri-supported", destination.num_options, destination.options);
    const char *result = cupsGetOption("device-uri", destination->num_options, destination->options);

//    cups_dinfo_t *destinationInfo = cupsCopyDestInfo(CUPS_HTTP_DEFAULT, destination);
//
//    cupsFreeDestInfo(destinationInfo);
//
//    const char *result = "";

    return result;
}
