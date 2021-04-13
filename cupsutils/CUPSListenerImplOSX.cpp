#include <notify.h>
#include <dispatch/dispatch.h>
#include "CUPSListenerImplOSX.hpp"
#include "CupsUtils.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace CupsUtilities
{

CUPSListenerImplOSX::CUPSListenerImplOSX(
    const onPrinterAdded& aPrinterAddedCallback,
    const onJobAdded& aJobAddedCallback)
        : _printerAddedCallback(aPrinterAddedCallback)
        , _jobAddedCallback(aJobAddedCallback)
        , _printerListChangedToken(0)
        , _jobChangedToken(0)
        , _printers()
        , _jobs()
{
    _listening_queue =
        dispatch_queue_create("cups_listening_queue", NULL);

    notify_register_dispatch("com.apple.printerListChange",
        &_printerListChangedToken, _listening_queue,
        ^(int token)
        {
            if (token == _printerListChangedToken)
            {
                this->printerListChanged();
            }
        });

    notify_register_dispatch("com.apple.jobChange",
        &_jobChangedToken, _listening_queue,
        ^(int token)
        {
            if (token == _jobChangedToken)
            {
                this->jobChanged();
            }
        });
}

CUPSListenerImplOSX::~CUPSListenerImplOSX()
{
    notify_cancel(_printerListChangedToken);
    notify_cancel(_jobChangedToken);
}

void CUPSListenerImplOSX::printerListChanged()
{
    if (_printerAddedCallback)
    {
        std::vector<std::string> currentPrintersList;

        {
            CupsUtils cupsUtils;
            currentPrintersList = cupsUtils.getPrintersNames();
        }

        if (_printers.size() != currentPrintersList.size())
        {
            if (_printers.size() < currentPrintersList.size())
            {
                for (int i = 0; i < currentPrintersList.size(); i++)
                {
                    if (i > _printers.size() || _printers.size() == 0)
                    {
                        std::string printerName = currentPrintersList.at(i);
                        _printerAddedCallback(printerName);
                    }
                }
            }
            _printers = currentPrintersList;
        }
    }
}

void CUPSListenerImplOSX::jobChanged()
{
    if (_jobAddedCallback)
    {
        std::vector<CupsJob> currentJobsList;

        {
            CupsUtils cupsUtils;
            currentJobsList = cupsUtils.getActiveJobs();
        }

        if (_jobs.size() != currentJobsList.size())
        {
            if (_jobs.size() < currentJobsList.size())
            {
                for (int i = 0; i < currentJobsList.size(); i++)
                {
                    if (i > _jobs.size() || _jobs.size() == 0)
                    {
                        CupsJob job = currentJobsList.at(i);
                        _jobAddedCallback(job);
                    }
                }
            }
            _jobs = currentJobsList;
        }
    }
}

}
