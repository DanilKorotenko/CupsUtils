#pragma once

#include <memory>

namespace CupsUtilities
{

class CupsNotificationReceiver
{
public:
    class Listener
    {
    public:
        using PtrT  = std::shared_ptr<Listener>;
        using WPtrT = std::weak_ptr<Listener>;

        virtual void printerListChanged() = 0;
        virtual void printerJobListChanged()  = 0;
    };
    using UPtrT = std::unique_ptr<CupsNotificationReceiver>;

    CupsNotificationReceiver(Listener::PtrT listener);
    ~CupsNotificationReceiver();

    CupsNotificationReceiver(const CupsNotificationReceiver&)       = delete;
    CupsNotificationReceiver(CupsNotificationReceiver&&)            = delete;
    CupsNotificationReceiver& operator=(CupsNotificationReceiver&)  = delete;
    CupsNotificationReceiver& operator=(CupsNotificationReceiver&&) = delete;

    class Impl;
private:
    std::unique_ptr<Impl> _impl;
};

} // namespace CupsUtilities

