//
//  main.cpp
//  getURIForPrinterName
//
//  Created by Danil Korotenko on 3/30/21.
//

#include <iostream>
#include "CupsUtils.hpp"

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <printer-name>" << std::endl;
        return 0;
    }

    std::string printerName = argv[1];

    CupsUtilities::CupsUtils cupsUtils;

    std::cout << cupsUtils.getOptionValueForPrinterWithName(printerName, CupsUtilities::kOptionNameDeviceURI) << std::endl;

    return 0;
}
