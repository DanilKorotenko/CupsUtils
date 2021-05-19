#include <notify.h>
#include <dispatch/dispatch.h>
#include "CUPSListenerImplOSX.hpp"
#include "CupsUtils.hpp"

////////////////////////////////////////////////////////////////////////////////
namespace CupsUtilities
{
    CUPSListenerImplOSX::CUPSListenerImplOSX()
        : _printerAddedCallback(nullptr)
        , _printerStateChangedCallback(nullptr)
        , _printerListEmptyCallback(nullptr)
        , _jobAddedCallback(nullptr)
        , _printerListChangedToken(0)
        , _jobChangedToken(0)
        , _printers()
        , _jobs()
    {
        _listening_queue =
            dispatch_queue_create("cups_listening_queue", NULL);
    }

    CUPSListenerImplOSX::~CUPSListenerImplOSX()
    {
        notify_cancel(_printerListChangedToken);
        notify_cancel(_jobChangedToken);
    }

    void CUPSListenerImplOSX::start()
    {
        // this notification will be sent on printer added, deleted, modified
        // see cupsd/main.cpp for details.
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

    void CUPSListenerImplOSX::setPrinterAddedCallback(
        const onPrinterAdded& aPrinterAddedCallback)
    {
        _printerAddedCallback = aPrinterAddedCallback;
    }

    void CUPSListenerImplOSX::setPrinterStateChangedCallback(
        const onPrinterStateChanged& aPrinterStateChangedCallback)
    {
        _printerStateChangedCallback = aPrinterStateChangedCallback;
    }

    void CUPSListenerImplOSX::setPrinterListEmpty(
        const onPrinterListEmpty& aPrinterListEmptyCallback)
    {
        _printerListEmptyCallback = aPrinterListEmptyCallback;
    }

    void CUPSListenerImplOSX::setJobAddedCallback(
        const onJobAdded& aJobAddedCallback)
    {
        _jobAddedCallback = aJobAddedCallback;
    }

    void CUPSListenerImplOSX::setJobChangedCallback(
        const onJobChanged& aJobChangedCallback)
    {
        _jobChangedCallback = aJobChangedCallback;
    }

#pragma mark -
////////////////////////////////////////////////////////////////////////////////

    void CUPSListenerImplOSX::printerListChanged()
    {
        std::vector<CupsPrinter> currentPrintersList;

        CupsUtils cupsUtils;

        currentPrintersList = cupsUtils.getPrinters();

        for (int i = 0; i < currentPrintersList.size(); i++)
        {
            CupsPrinter currentPrinter = currentPrintersList.at(i);

            auto printerIt = std::find_if(_printers.begin(), _printers.end(),
                [&](CupsPrinter aPrinter)
                {
                    return currentPrinter.name == aPrinter.name;
                });

            if (printerIt == _printers.end())
            {
                if (_printerAddedCallback)
                {
                    _printerAddedCallback(currentPrinter.name);
                }
            }
            else
            {
                CupsPrinter printer = *printerIt;
                if (std::strcmp(
                    currentPrinter.stateReasons.c_str(),
                    printer.stateReasons.c_str()) != 0)
                {
                    if (_printerStateChangedCallback)
                    {
                        _printerStateChangedCallback(currentPrinter.name);
                    }
                }
            }
        }

        if (_printerListEmptyCallback)
        {
            if (currentPrintersList.empty())
            {
                _printerListEmptyCallback();
            }
        }

        _printers = currentPrintersList;
    }

    void CUPSListenerImplOSX::jobChanged()
    {
        std::vector<CupsJob::PtrT> currentJobsList;

        {
            CupsUtils cupsUtils;
            currentJobsList = cupsUtils.getActiveJobs();
        }

        if (_jobChangedCallback)
        {
            _jobChangedCallback(currentJobsList);
        }

        if (_jobs.size() < currentJobsList.size())
        {
            for (int i = 0; i < currentJobsList.size(); i++)
            {
                if (i > _jobs.size() || _jobs.empty())
                {
                    if (_jobAddedCallback)
                    {
                        CupsJob::PtrT job = currentJobsList.at(i);
                        _jobAddedCallback(job);
                    }
                }
            }
        }

        _jobs = currentJobsList;
    }
}
