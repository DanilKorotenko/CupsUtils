//
//  main.cpp
//  cupsListener
//
//  Created by Danil Korotenko on 4/13/21.
//

#include <iostream>
#include <dispatch/dispatch.h>
#include "ICUPSListener.hpp"

void onPrinterAdded(CupsUtilities::CupsUtils& aCupsUtils, const CupsUtilities::CupsPrinter& aPrinter)
{
    std::cout << "Printer Added: " << aPrinter.name.c_str() << std::endl;
}

void onPrinterStateChanged(CupsUtilities::CupsUtils& aCupsUtils, const CupsUtilities::CupsPrinter& aPrinter)
{
    std::cout << "Printer State Changed: " << aPrinter.name.c_str() << std::endl;
}

void onPrinterListEmpty()
{
    std::cout << "Printer List Empty: " << std::endl;
}

void onJobAdded(const CupsUtilities::CupsJob::PtrT& aCupsJob)
{
    std::cout << "Job Added: " << aCupsJob->toString() << std::endl;
}

void onJobChanged(std::vector<CupsUtilities::CupsJob::PtrT> aJobList)
{
    for (CupsUtilities::CupsJob::PtrT job: aJobList)
    {
        std::cout << "Job Changed: " << job->toString() << std::endl;
    }
}

int main(int argc, const char * argv[])
{
    CupsUtilities::ICUPSListener::UPtrT cupsListener(
        CupsUtilities::createCupsListener()
    );

    using onJobChanged = std::function<void(std::vector<CupsUtilities::CupsJob::PtrT>)>;

    cupsListener->setPrinterAddedCallback(std::bind(&onPrinterAdded,
        std::placeholders::_1, std::placeholders::_2));
    cupsListener->setPrinterStateChangedCallback(std::bind(&onPrinterStateChanged,
        std::placeholders::_1, std::placeholders::_2));
    cupsListener->setPrinterListEmpty(std::bind(&onPrinterListEmpty));
    cupsListener->setJobAddedCallback(std::bind(&onJobAdded, std::placeholders::_1));
//    cupsListener->setJobChangedCallback(std::bind(&onJobChanged, std::placeholders::_1));

    dispatch_main();

    return 0;
}
