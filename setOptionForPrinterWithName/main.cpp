//
//  main.cpp
//  setOptionForPrinterWithName
//
//  Created by Danil Korotenko on 4/1/21.
//

#include <iostream>
#include "cupsutils.hpp"

int main(int argc, const char * argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: " << argv[0] << " <printer-name> <option-name> <option-value>" << std::endl;
        return 0;
    }

    std::string printerName = argv[1];
    std::string optionName = argv[2];
    std::string optionValue = argv[3];

    CupsUtilities::CupsUtils cupsUtils;

    std::string previousOptionValue = cupsUtils.getOptionValueForPrinterWithName(
        printerName, optionName);

    std::cout << "previousOptionValue: " << previousOptionValue.c_str() << std::endl;

    bool setOptionResult = cupsUtils.setOptionForPrinterWithName(printerName,
        { optionName, optionValue });

    std::cout << (setOptionResult ?
        "The option was successfuly set." : "Unable to set option.") << std::endl;


    return 0;
}
