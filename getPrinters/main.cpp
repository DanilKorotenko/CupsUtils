//
//  main.cpp
//  getPrinters
//
//  Created by Danil Korotenko on 3/30/21.
//

#include <iostream>
#include "CupsUtils.hpp"

int main(int argc, const char * argv[])
{
    CupsUtilities::CupsUtils cupsUtils;

    std::vector<CupsUtilities::CupsPrinter> printers = cupsUtils.getPrinters();

    for (std::vector<CupsUtilities::CupsPrinter>::iterator it = printers.begin();
        it != printers.end(); it++)
    {
        std::cout << (*it).name.c_str() << std::endl;
    }

    return 0;
}
