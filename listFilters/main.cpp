//
//  main.cpp
//  listFilters
//
//  Created by Danil Korotenko on 4/5/21.
//

#include <iostream>
#include "PPDUtils.hpp"

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <printer-name>" << std::endl;
        return 0;
    }

    std::string printerName = argv[1];

    PPDUtils ppdUtils;

    std::cout << ppdUtils.getPPDFilePathForPrinterName(printerName) << std::endl;

    return 0;
}
