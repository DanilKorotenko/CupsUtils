//
//  main.cpp
//  cupsListener
//
//  Created by Danil Korotenko on 4/13/21.
//

#include <iostream>
#include <dispatch/dispatch.h>
#include "ICUPSListener.hpp"

void onPrinterAdded(const std::string& aPrinterName)
{
    std::cout << "Printer Added: " << aPrinterName.c_str() << std::endl;
}

void onJobAdded(const CupsUtilities::CupsJob& aCupsJob)
{
    std::cout << "Job Added: " << aCupsJob.job_id << " : " << aCupsJob.title.c_str() << std::endl;
}

int main(int argc, const char * argv[])
{
    CupsUtilities::ICUPSListener::UPtrT cupsListener(
        CupsUtilities::createCupsListener()
    );

    cupsListener->setPrinterAddedCallback(std::bind(&onPrinterAdded, std::placeholders::_1));
    cupsListener->setJobAddedCallback(std::bind(&onJobAdded, std::placeholders::_1));

    dispatch_main();

    return 0;
}
