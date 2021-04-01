//
//  main.cpp
//  getPrinters
//
//  Created by Danil Korotenko on 3/30/21.
//

#include <iostream>
#include "cupsutils/cupsutils.hpp"

//const char *getDestinationURI(cups_dest_t destination)
//{
//    const char *result = cupsGetOption("device-uri", destination.num_options, destination.options);
//
//    return result;
//}

int main(int argc, const char * argv[])
{
    CupsUtils cupsUtils;

    std::vector<std::string> printerNames = cupsUtils.getPrintersNames();

    for (std::vector<std::string>::iterator it = printerNames.begin();
        it != printerNames.end(); it++)
    {
//        std::cout << destinations_data.destinations[i].name << " URI:" << getDestinationURI(destinations_data.destinations[i]) << std::endl;
        std::cout << (*it).c_str() << std::endl;
    }

    return 0;
}
