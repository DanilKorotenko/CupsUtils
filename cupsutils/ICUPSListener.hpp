#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "CupsUtilsTypes.hpp"
#include "CupsJob.hpp"

namespace CupsUtilities
{
    // 1st argument is added printer name
    using onPrinterAdded = std::function<void(const std::string&)>;
    using onPrinterStateChanged = std::function<void(const std::string&)>;
    using onPrinterListEmpty = std::function<void()>;
    using onJobAdded = std::function<void(const CupsJob::PtrT&)>;
    using onJobChanged = std::function<void(std::vector<CupsJob::PtrT>)>;

    class ICUPSListener
    {
    public:
        using UPtrT = std::unique_ptr<ICUPSListener>;

        virtual void start() = 0;

        virtual void setPrinterAddedCallback(
            const onPrinterAdded& aPrinterAddedCallback) = 0;
        virtual void setPrinterStateChangedCallback(
            const onPrinterStateChanged& aPrinterStateChangedCallback) = 0;
        virtual void setPrinterListEmpty(
            const onPrinterListEmpty& aPrinterListEmptyCallback) = 0;
        virtual void setJobAddedCallback(const onJobAdded& aJobAddedCallback) = 0;
        virtual void setJobChangedCallback(const onJobChanged& aJobChangedCallback) = 0;
    };

    ICUPSListener::UPtrT createCupsListener();
}
