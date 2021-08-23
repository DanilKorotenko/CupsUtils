#pragma once

#include "ICupsNotificationCenter.hpp"
#include "CupsNotificationReceiver.hpp"

namespace CupsUtilities
{

class CupsNotificationCenter: public std::enable_shared_from_this<CupsNotificationCenter>,
                              public CupsNotificationReceiver::Listener,
                              public ICupsNotificationCenter
{
public:
    explicit CupsNotificationCenter(ICupsNotificationCenter::Listener::PtrT listener);
    ~CupsNotificationCenter() override;

    void start() override;

private:
    void printerListChanged()     override;
    void printerJobListChanged()  override;

    static bool isJobPresent(const CupsJob::PtrT& job, std::vector<CupsJob::PtrT> jobs);
private:
    CupsNotificationReceiver::UPtrT _notificationProducer;
    std::vector<CupsPrinter>        _printers;
    std::vector<CupsJob::PtrT>      _jobs;
};

} // namespace CupsUtilities
