//
//  main.cpp
//  listOptionsForPrinterName
//
//  Created by Danil Korotenko on 4/1/21.
//

#include <iostream>
#include "cupsutils/cupsutils.hpp"

int main(int argc, const char * argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <printer-name>" << std::endl;
        return 0;
    }

    std::string printerName = argv[1];

    CupsUtils cupsUtils;

    std::vector<CupsOption> printerOptions =
        cupsUtils.getListOfOptionsForPrinterWithName(printerName);

    for (std::vector<CupsOption>::iterator it = printerOptions.begin();
        it != printerOptions.end(); it++)
    {
        std::cout << (*it).name.c_str() << " : " << (*it).value.c_str() << std::endl;
    }

    return 0;
}
