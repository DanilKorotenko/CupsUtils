#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "CupsUtilsTypes.hpp"

namespace CupsUtilities
{
    // 1st argument is added printer name
    using onPrinterAdded = std::function<void(const std::string&)>;
    using onPrinterListEmpty = std::function<void()>;
    // 1st argument is added job uri
    using onJobAdded = std::function<void(const CupsJob&)>;

    class ICUPSListener
    {
    public:
        using UPtrT = std::unique_ptr<ICUPSListener>;

        virtual void setPrinterAddedCallback(const onPrinterAdded& aPrinterAddedCallback) = 0;
        virtual void setPrinterListEmpty(const onPrinterListEmpty& aPrinterListEmptyCallback) = 0;
        virtual void setJobAddedCallback(const onJobAdded& aJobAddedCallback) = 0;
    };

    ICUPSListener::UPtrT createCupsListener();
}
