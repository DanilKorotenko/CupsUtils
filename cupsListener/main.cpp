//
//  main.cpp
//  cupsListener
//
//  Created by Danil Korotenko on 4/13/21.
//

#include <iostream>
#include <dispatch/dispatch.h>
#include "../cupsutils/ICupsNotificationCenter.hpp"


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

class MyListener: public CupsUtilities::ICupsNotificationCenter::Listener
{
public:
    using PtrT = std::shared_ptr<MyListener>;

    void printerAdded(CupsUtilities::CupsUtils &aCupsUtils, const CupsUtilities::CupsPrinter &aPrinter)
    {
        std::cout << "printerAdded: " << aPrinter.name.c_str() << std::endl;
    }

    void printerModified(CupsUtilities::CupsUtils &aCupsUtils, const CupsUtilities::CupsPrinter &aPrinter)
    {
        std::cout << "printerModified: " << aPrinter.name.c_str() << std::endl;
    }

    void printerListEmpty()
    {
        std::cout << "printerListEmpty" << std::endl;
    }

    void jobAdded(CupsUtilities::CupsJob::PtrT &aCupsJob)
    {
        std::cout << "Job Added: " << aCupsJob->toString() << std::endl;
    }

    void jobChanged(CupsUtilities::CupsJob::PtrT &aCupsJob)
    {
        std::cout << "jobChanged: " << aCupsJob->toString() << std::endl;
    }

    void jobFinished(CupsUtilities::CupsJob::PtrT &aCupsJob)
    {
        std::cout << "jobFinished: " << aCupsJob->toString() << std::endl;
    }
};

int main(int argc, const char * argv[])
{
    MyListener::PtrT myListener = std::make_shared<MyListener>();

    CupsUtilities::ICupsNotificationCenter::PtrT cupsNotificationCenter = CupsUtilities::ICupsNotificationCenter::create(myListener);
    cupsNotificationCenter->start();

    dispatch_main();

    return 0;
}
