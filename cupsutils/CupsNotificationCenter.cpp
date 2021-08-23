#include "CupsNotificationCenter.hpp"
#include "CupsUtils.hpp"
#include <algorithm>
#include "std_make_unique.h"

namespace CupsUtilities
{

CupsNotificationCenter::CupsNotificationCenter(ICupsNotificationCenter::Listener::PtrT listener):
    ICupsNotificationCenter(std::move(listener))
    , _notificationProducer(nullptr)
    , _printers()
    , _jobs()
{
}

CupsNotificationCenter::~CupsNotificationCenter()
{
    _notificationProducer.reset();
}

void CupsNotificationCenter::start()
{
    printerListChanged();
    _notificationProducer = std::make_unique<CupsNotificationReceiver>(shared_from_this());
}

void CupsNotificationCenter::printerListChanged()
{
    CupsUtils cupsUtils;
    auto currentPrintersList = cupsUtils.getPrinters();
    bool update = false;

    if (auto listener = _listener.lock())
    {
        for (auto& currentPrinter: currentPrintersList)
        {
            auto it = std::find_if(_printers.begin(), _printers.end(),
                                          [&](const CupsPrinter& inst)
                                          {
                                              return currentPrinter.uri == inst.uri;
                                          });
            if (it == _printers.end())
            {
                listener->printerAdded(cupsUtils, currentPrinter);
                update = true;
            }
            else
            {
                if (currentPrinter.isModified(*it))
                {
                    listener->printerModified(cupsUtils, currentPrinter);
                    update = true;
                }
            }
        }

        if (currentPrintersList.empty())
        {
            listener->printerListEmpty();
        }
    }

    if (update)
    {
        currentPrintersList = CupsUtils().getPrinters();
    }
    _printers.swap(currentPrintersList);
}

void CupsNotificationCenter::printerJobListChanged()
{
    CupsUtils cupsUtils;
    auto currentJobsList = cupsUtils.getActiveJobs();

    if (auto listener = _listener.lock())
    {
        for (auto& jobToCheck: currentJobsList)
        {
            if (!isJobPresent(jobToCheck, _jobs))
            {
                listener->jobAdded(jobToCheck);
            }
            else
            {
                listener->jobChanged(jobToCheck);
            }
        }

        for (auto& jobToCheck: _jobs)
        {
            if (!isJobPresent(jobToCheck, currentJobsList))
            {
                listener->jobFinished(jobToCheck);
            }
        }
    }
    _jobs.swap(currentJobsList);
}

bool CupsNotificationCenter::isJobPresent(const CupsJob::PtrT& job, std::vector<CupsJob::PtrT> jobs)
{
    auto it = std::find_if(jobs.begin(), jobs.end(),
                           [&](const CupsJob::PtrT target)
                           {
                               return target->jobId == job->jobId;
                           });

    return jobs.end() != it;
}

ICupsNotificationCenter::PtrT ICupsNotificationCenter::create(ICupsNotificationCenter::Listener::PtrT listener)
{
    return std::make_shared<CupsNotificationCenter>(listener);
}

} // namespace CupsUtilities
