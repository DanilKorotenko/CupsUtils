#include <notify.h>
#include <dispatch/dispatch.h>
#include "CUPSListenerImplOSX.hpp"
#include "CupsUtils.hpp"

////////////////////////////////////////////////////////////////////////////////

CUPSListenerImplOSX::CUPSListenerImplOSX(
    const onPrinterAdded& aPrinterAddedCallback,
    const onJobAdded& aJobAddedCallback)
        : _printerAddedCallback(aPrinterAddedCallback)
        , _jobAddedCallback(aJobAddedCallback)
        , _printerListChangedToken(0)
        , _jobChangedToken(0)
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
        std::string printerName;

        _printerAddedCallback(printerName);
    }
}

void CUPSListenerImplOSX::jobChanged()
{
    if (_jobAddedCallback)
    {
        std::string jobURI;
        _jobAddedCallback(jobURI);
    }
}
