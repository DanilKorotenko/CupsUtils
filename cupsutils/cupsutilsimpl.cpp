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
