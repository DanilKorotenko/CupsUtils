#include "CupsNotificationReceiver.hpp"

#include <thread>
#include <dbus/dbus.h>
#include "useful/ScopedFlagHandler.hpp"
#include <iostream>
#include <atomic>
#include <sstream>
#include "logger/Logger.h"

namespace CupsUtilities
{

namespace
{
    const char* filterIf = "type=signal,interface=org.cups.cupsd.Notifier";
}

class DbusError: std::runtime_error
{
public:
    explicit DbusError(const char* msg): std::runtime_error(msg) {}
};

class CupsNotificationReceiver::Impl
{
public:
    friend DBusHandlerResult monitorFilterCb(DBusConnection*, DBusMessage*, void*);

    Impl(CupsNotificationReceiver::Listener::PtrT listener):
            _listener(listener)
            , _connection(nullptr)
            , _workerRunning(false)
            , _interruption(false)
    {
        init();
    }

    ~Impl()
    {
        _interruption.store(true);
        _workerRunning.wait(false);
    }

    static DBusHandlerResult monitorFilterCb(DBusConnection *connection, DBusMessage *message, void *userData)
    {
        if (dbus_message_is_signal (message, DBUS_INTERFACE_LOCAL, "Disconnected"))
        {
            if (userData)
            {
                auto inst = reinterpret_cast<CupsNotificationReceiver::Impl*>(userData);
                inst->disconnected();
            }
        }

        if (userData)
        {
            auto inst = reinterpret_cast<CupsNotificationReceiver::Impl*>(userData);
            auto namePtr = dbus_message_get_member(message);
            if(namePtr)
            {
                std::string name(namePtr);
                inst->handleMessage(name);
            }
        }

        // Monitors must not allow libdbus to reply to messages, so we eat the message.
        return DBUS_HANDLER_RESULT_HANDLED;
    }

private:

    void init()
    {
        initDispatchThr();
        _workerRunning.wait(true);
    }

    void connectionInit()
    {
        DBusError error;
        dbus_error_init (&error);

        // Connect to BUS.
        _connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
        if (_connection == NULL)
        {
            std::stringstream ss;
            ss << "Failed to open connection to dbus system " << error.name << " : " << error.message;
            dbus_error_free(&error);
            throw DbusError(ss.str().c_str()) ;
        }
        dbus_error_free (&error);

        dbus_connection_set_route_peer_messages (_connection, TRUE);
        // Setup callback function.
        if (!dbus_connection_add_filter (_connection, monitorFilterCb, this, NULL))
        {
            throw DbusError("Couldn't add callback function!");
        }
    }

    void connectionClose()
    {
        if(_connection)
        {
            dbus_connection_unref(_connection);
            _connection = nullptr;
        }
    }

    void startMonitor ()
    {
        DBusMessage *requestMsg;
        requestMsg = prepareRequest();
        sendRequest(requestMsg);
    }

    DBusMessage* prepareRequest()
    {
        DBusMessage *requestMsg;
        dbus_uint32_t zero = 0;
        DBusMessageIter appender, array_appender;

        requestMsg = dbus_message_new_method_call (DBUS_SERVICE_DBUS, DBUS_PATH_DBUS,
                                                   DBUS_INTERFACE_MONITORING, "BecomeMonitor");

        dbus_message_iter_init_append (requestMsg, &appender);

        if (!dbus_message_iter_open_container (&appender, DBUS_TYPE_ARRAY, "s", &array_appender))
        {
            throw DbusError("Opening string array error.");
        }

        if (!dbus_message_iter_append_basic (&array_appender, DBUS_TYPE_STRING, &filterIf))
        {
            throw DbusError("Adding filter to array error.");
        }

        if (!dbus_message_iter_close_container (&appender, &array_appender) ||
            !dbus_message_iter_append_basic (&appender, DBUS_TYPE_UINT32, &zero))
        {
            throw DbusError("Finishing arguments error.");
        }

        return requestMsg;
    }

    void sendRequest(DBusMessage* request)
    {
        DBusError error = DBUS_ERROR_INIT;
        DBusMessage *responseMsg;

        responseMsg = dbus_connection_send_with_reply_and_block(_connection, request, -1, &error);

        if (responseMsg != NULL)
        {
            dbus_message_unref (responseMsg);
        }
        else if (dbus_error_has_name (&error, DBUS_ERROR_UNKNOWN_INTERFACE))
        {
            dbus_error_free (&error);
            throw DbusError( "dbus-monitor: unable to enable new-style monitoring, "
                             "your dbus-daemon is too old. Falling back to eavesdropping.") ;
        }
        else
        {
            std::stringstream ss;
            ss << "Unable to enable new-style monitoring: " << error.name << " : " << error.message
               << ". Falling back to eavesdropping.";
            dbus_error_free(&error);
            throw DbusError(ss.str().c_str()) ;
        }

        dbus_message_unref (request);
    }

    void initDispatchThr()
    {
        std::thread tmp(&Impl::process, this);
        tmp.detach();
    }

    void process()
    {
        auto executionFlag = _workerRunning.changeFlag();

        try
        {
            connectionInit();
            startMonitor();

            while(dbus_connection_read_write_dispatch(_connection, 100) && !_interruption.load());
            connectionClose();

        }
        catch (const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
        }
    }

    void disconnected()
    {
        ERROR_LOG  << "Dbus disconnected.";
    }

    void handleMessage(const std::string& name)
    {
        TRACE_LOG << "Message received: " << name;
        if (auto listener = _listener.lock())
        {
            if (name.rfind("Job", 0) == 0)
            {
                listener->printerJobListChanged();
            }
            else if (name.rfind("Printer", 0) == 0)
            {
                listener->printerListChanged();
            }
        }
    }


private:
    CupsNotificationReceiver::Listener::WPtrT _listener;

    DBusConnection*  _connection;
    FlagHandler      _workerRunning;
    std::atomic_bool _interruption;
};

CupsNotificationReceiver::CupsNotificationReceiver(Listener::PtrT listener):
        _impl(new CupsNotificationReceiver::Impl(listener))
{
}

CupsNotificationReceiver::~CupsNotificationReceiver()
{
}

} // namespace CupsUtilities
