//
//  main.cpp
//  getPrinters
//
//  Created by Danil Korotenko on 3/30/21.
//

#include <iostream>
#include <cups/cups.h>

typedef struct
{
    int number_of_destinations;
    cups_dest_t *destinations;
} DestinationsData;

int destinationsCallback(DestinationsData *destinations_data, unsigned flags, cups_dest_t *dest)
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

int getDestinations(cups_ptype_t type, cups_ptype_t mask, DestinationsData *aDestinationsData)
{
    if (!cupsEnumDests(CUPS_DEST_FLAGS_NONE, 5000, NULL, type,
        mask, (cups_dest_cb_t)destinationsCallback,
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

const char *getDestinationURI(cups_dest_t destination)
{
    const char *result = cupsGetOption("printer-uri-supported", destination.num_options, destination.options);

    return result;
}

int main(int argc, const char * argv[])
{
    std::cout << "Hello, Printers!\n";

    DestinationsData destinations_data = { 0, NULL };

    getDestinations(CUPS_PRINTER_LOCAL, CUPS_PRINTER_DISCOVERED, &destinations_data);

    std::cout << "Found: " << destinations_data.number_of_destinations << " destinations" << std::endl;

    for (int i = 0; i < destinations_data.number_of_destinations; i++)
    {
//        std::cout << destinations_data.destinations[i].name << " URI:" << getDestinationURI(destinations_data.destinations[i]) << std::endl;
        std::cout << destinations_data.destinations[i].name << std::endl;
    }

    cupsFreeDests(destinations_data.number_of_destinations, destinations_data.destinations);

    return 0;
}
