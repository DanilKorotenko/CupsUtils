//
//  main.cpp
//  listPrintersPolicies
//
//  Created by Danil Korotenko on 4/21/21.
//

#include <iostream>
#include "CupsUtils.hpp"

int main(int argc, const char * argv[])
{
    CupsUtilities::CupsUtils cupsUtils;

    std::vector<std::string> printerNames = cupsUtils.getPrintersNames();

    for (std::vector<std::string>::iterator it = printerNames.begin();
        it != printerNames.end(); it++)
    {
        std::cout << (*it).c_str() << std::endl;
    }

    return 0;
}
