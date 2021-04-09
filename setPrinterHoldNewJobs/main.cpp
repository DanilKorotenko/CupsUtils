//
//  main.cpp
//  setPrinterHoldNewJobs
//
//  Created by Danil Korotenko on 4/9/21.
//

#include <iostream>
#include "cupsutils.hpp"

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <printer-name>" << std::endl;
        return 0;
    }

    std::string printerName = argv[1];

    CupsUtils cupsUtils;

    std::cout << (cupsUtils.setPrinterHoldNewJobs(printerName) ?
        "Printer is holding new jobs." : "Error occured.") << std::endl;

    return 0;
}
