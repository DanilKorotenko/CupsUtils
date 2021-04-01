//
//  main.cpp
//  getPrinters
//
//  Created by Danil Korotenko on 3/30/21.
//

#include <iostream>
#include "cupsutils/cupsutils.hpp"

int main(int argc, const char * argv[])
{
    CupsUtils cupsUtils;

    std::vector<std::string> printerNames = cupsUtils.getPrintersNames();

    for (std::vector<std::string>::iterator it = printerNames.begin();
        it != printerNames.end(); it++)
    {
        std::cout << (*it).c_str() << std::endl;
    }

    return 0;
}
