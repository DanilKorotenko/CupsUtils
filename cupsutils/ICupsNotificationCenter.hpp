#pragma once

#include <string>
#include <memory>
#include <functional>
#include "CupsUtilsTypes.hpp"
#include "CupsUtils.hpp"

namespace CupsUtilities
{

class ICupsNotificationCenter
{
public:
    class Listener
    {
    public:
        using PtrT = std::shared_ptr<Listener>;
        using WPtrT = std::weak_ptr<Listener>;

        virtual void printerAdded(CupsUtils&, const CupsPrinter&) {};
        virtual void printerModified(CupsUtils&, const CupsPrinter&) {};
        virtual void printerListEmpty() {};
        virtual void jobAdded(CupsJob::PtrT&) {};
        virtual void jobChanged(CupsJob::PtrT&) {};
        virtual void jobFinished(CupsJob::PtrT&) {};
    };

    using PtrT = std::shared_ptr<ICupsNotificationCenter>;

    virtual void start() = 0;

    explicit ICupsNotificationCenter(Listener::PtrT listener):
            _listener(listener)
    {
    }

    virtual ~ICupsNotificationCenter() = default;

    inline void setListener(Listener::PtrT listener)
    {
        _listener = listener;
    }

    static ICupsNotificationCenter::PtrT create(Listener::PtrT listener);

protected:
    Listener::WPtrT _listener;
};

} // namespace CupsUtilities
