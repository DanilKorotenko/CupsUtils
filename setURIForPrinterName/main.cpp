//
//  main.cpp
//  setURIForPrinterName
//
//  Created by Danil Korotenko on 3/30/21.
//

#include <iostream>
#include "cupsutils.hpp"

int main(int argc, const char * argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0] << " <printer-name> <new-device-URI>" << std::endl;
        return 0;
    }

    std::string printerName = argv[1];
    std::string newDeviceURI = argv[2];

    CupsUtils cupsUtils;

    if (!cupsUtils.checkURI(newDeviceURI))
    {
        std::cout << "unable to set uri" << std::endl;
        return 1;
    }

    std::string previousDeviceURI = cupsUtils.getOptionValueForPrinterWithName(
        printerName, kDeviceURIOptionName);

    std::cout << "previousDeviceURI: " << previousDeviceURI.c_str() << std::endl;

    cupsUtils.setOptionForPrinterWithName(printerName,
        { kDeviceURIOptionName, newDeviceURI });

    return 0;
}
