#pragma once

#include "ICUPSListener.hpp"
#include <dispatch/dispatch.h>

namespace CupsUtilities
{
    class CUPSListenerImplOSX: public ICUPSListener
    {
    public:
        CUPSListenerImplOSX(
            const onPrinterAdded& aPrinterAddedCallback,
            const onJobAdded& aJobAddedCallback);
        ~CUPSListenerImplOSX();

    private:
        void printerListChanged();
        void jobChanged();

        onPrinterAdded      _printerAddedCallback;
        onJobAdded          _jobAddedCallback;

        dispatch_queue_t    _listening_queue;
        int                 _printerListChangedToken;
        int                 _jobChangedToken;

        std::vector<std::string> _printers;
        std::vector<CupsJob>     _jobs;
    };
}
