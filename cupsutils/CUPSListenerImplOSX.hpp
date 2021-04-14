#pragma once

#include "ICUPSListener.hpp"
#include <dispatch/dispatch.h>

namespace CupsUtilities
{
    class CUPSListenerImplOSX: public ICUPSListener
    {
    public:
        CUPSListenerImplOSX();
        ~CUPSListenerImplOSX();

        void setPrinterAddedCallback(const onPrinterAdded& aPrinterAddedCallback) override;
        void setPrinterListEmpty(const onPrinterListEmpty& aPrinterListEmptyCallback) override;
        void setJobAddedCallback(const onJobAdded& aJobAddedCallback) override;

    private:
        void printerListChanged();
        void jobChanged();

        onPrinterAdded      _printerAddedCallback;
        onPrinterListEmpty  _printerListEmptyCallback;
        onJobAdded          _jobAddedCallback;

        dispatch_queue_t    _listening_queue;
        int                 _printerListChangedToken;
        int                 _jobChangedToken;

        std::vector<std::string> _printers;
        std::vector<CupsJob>     _jobs;
    };
}
