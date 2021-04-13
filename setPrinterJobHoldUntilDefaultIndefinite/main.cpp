//
//  main.cpp
//  setPrinterJobHoldUntilDefaultIndefinite
//
//  Created by Danil Korotenko on 4/13/21.
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

    std::string previousOptionValue = cupsUtils.getOptionValueForPrinterWithName(
        printerName, kJobHoldUntilDefaultOptionName);

    std::cout << "previousOptionValue: " << previousOptionValue.c_str() << std::endl;

    bool setOptionResult = cupsUtils.setOptionForPrinterWithName(printerName,
        { kJobHoldUntilDefaultOptionName, kJobHoldUntilDefaultOptionValueIndefinite });

    std::cout << (setOptionResult ?
        "The option was successfuly set." : "Unable to set option.") << std::endl;

    return 0;
}
