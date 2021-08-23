#include "CupsNotificationReceiver.hpp"
#include <dispatch/dispatch.h>
#include <notify.h>
#include <notify_keys.h>

namespace CupsUtilities
{

class CupsNotificationReceiver::Impl
{
public:

    Impl(CupsNotificationReceiver::Listener::PtrT listener):
            _listener(listener)
            , _listeningQueue()
            , _printerListChangedToken(0)
            , _jobChangedToken(0)
    {
        _listeningQueue = dispatch_queue_create("cupsListeningQueue", DISPATCH_QUEUE_SERIAL);
        init();
    }

    ~Impl()
    {
        notify_cancel(_printerListChangedToken);
        notify_cancel(_jobChangedToken);
    }

private:

    void init()
    {
        // this notification will be sent on printer added, deleted, modified
        // see cupsd/main.cpp for details.
        notify_register_dispatch("com.apple.printerListChange",
                                 &_printerListChangedToken, _listeningQueue,
                                 ^(int token)
                                 {
                                     if (token == this->_printerListChangedToken)
                                     {
                                         if (auto listener = this->_listener.lock())
                                         {
                                             listener->printerListChanged();
                                         }
                                     }
                                 });

        notify_register_dispatch("com.apple.jobChange",
                                 &_jobChangedToken, _listeningQueue,
                                 ^(int token)
                                 {
                                     if (token == this->_jobChangedToken)
                                     {
                                         if (auto listener = this->_listener.lock())
                                         {
                                             listener->printerJobListChanged();
                                         }
                                     }
                                 });
    }

private:
    CupsNotificationReceiver::Listener::WPtrT _listener;
    dispatch_queue_t                        _listeningQueue;
    int                                     _printerListChangedToken;
    int                                     _jobChangedToken;
};

CupsNotificationReceiver::CupsNotificationReceiver(Listener::PtrT listener):
        _impl(new CupsNotificationReceiver::Impl(listener))
{
}

CupsNotificationReceiver::~CupsNotificationReceiver()
{
}

} // namespace CupsUtilities
