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

        void start() override;

        void setPrinterAddedCallback(
            const onPrinterAdded& aPrinterAddedCallback) override;
        void setPrinterStateChangedCallback(
            const onPrinterStateChanged& aPrinterStateChangedCallback) override;
        void setPrinterListEmpty(
            const onPrinterListEmpty& aPrinterListEmptyCallback) override;
        void setJobAddedCallback(const onJobAdded& aJobAddedCallback) override;
        void setJobChangedCallback(const onJobChanged& aJobChangedCallback) override;

    private:
        void printerListChanged();
        void jobChanged();

        onPrinterAdded          _printerAddedCallback;
        onPrinterStateChanged   _printerStateChangedCallback;
        onPrinterListEmpty      _printerListEmptyCallback;
        onJobAdded              _jobAddedCallback;
        onJobChanged            _jobChangedCallback;

        dispatch_queue_t    _listening_queue;
        int                 _printerListChangedToken;
        int                 _jobChangedToken;

        std::vector<CupsPrinter>    _printers;
        std::vector<CupsJob::PtrT>  _jobs;
    };
}
