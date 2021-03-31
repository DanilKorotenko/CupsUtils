//
//  main.cpp
//  setURIForPrinterName
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

static int set_printer_options(
    int num_options,
    cups_option_t *options,
    const char *printerUri)
{
    ipp_t *request = ippNewRequest(IPP_OP_CUPS_ADD_MODIFY_PRINTER);

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, printerUri);
//    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());

    cupsEncodeOptions2(request, num_options, options, IPP_TAG_PRINTER);

    ippDelete(cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/admin/"));

    if (cupsLastError() > IPP_STATUS_OK_CONFLICTING)
    {
        std::cout << "Cups last error: " << cupsLastErrorString() << std::endl;
        return (1);
    }

    return (0);
}

int main(int argc, const char * argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <printer-name> <new-URI>" << std::endl;
        return 0;
    }

    std::string printerName = argv[1];
    std::string newDeviceURI = argv[2];

    DestinationsData destinations_data = { 0, NULL };

    getDestinations(CUPS_PRINTER_LOCAL, CUPS_PRINTER_DISCOVERED, &destinations_data);

    cups_dest_t *printerDestination = cupsGetDest(printerName.c_str(), NULL,
        destinations_data.number_of_destinations, destinations_data.destinations);

    std::string previousDeviceURI = cupsGetOption("device-uri",
        printerDestination->num_options,
        printerDestination->options);

    std::string printerURI = cupsGetOption("printer-uri-supported",
        printerDestination->num_options,
        printerDestination->options);

    std::cout << "previousDeviceURI: " << previousDeviceURI.c_str() << std::endl;

    std::cout << "remove option" << std::endl;

    printerDestination->num_options = cupsRemoveOption("device-uri",
        printerDestination->num_options, &printerDestination->options);

    std::cout << "num_options: " << printerDestination->num_options << std::endl;

    std::cout << "add option" << std::endl;

    printerDestination->num_options = cupsAddOption("device-uri",
        newDeviceURI.c_str(),
        printerDestination->num_options, &printerDestination->options);

    std::cout << "num_options: " << printerDestination->num_options << std::endl;

    std::string newDeviceURIFromOptions = cupsGetOption("device-uri",
        printerDestination->num_options,
        printerDestination->options);

    std::cout << "newDeviceURIFromOptions: " <<
        newDeviceURIFromOptions.c_str() << std::endl;

    set_printer_options(printerDestination->num_options,
        printerDestination->options,
        printerURI.c_str());

    cupsFreeDests(destinations_data.number_of_destinations, destinations_data.destinations);

    return 0;
}
