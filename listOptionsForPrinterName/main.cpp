//
//  main.cpp
//  listOptionsForPrinterName
//
//  Created by Danil Korotenko on 4/1/21.
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

    std::vector<CupsUtilities::CupsOption> printerOptions =
        cupsUtils.getListOfOptionsForPrinterWithName(printerName);

    for (std::vector<CupsUtilities::CupsOption>::iterator it = printerOptions.begin();
        it != printerOptions.end(); it++)
    {
        std::cout << (*it).name.c_str() << " : " << (*it).value.c_str() << std::endl;
    }

    return 0;
}
