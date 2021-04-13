#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

// 1st argument is added printer name
using onPrinterAdded = std::function<void(const std::string&)>;
// 1st argument is added job uri
using onJobAdded = std::function<void(const std::string&)>;

class ICUPSListener
{
public:
    using UPtrT = std::unique_ptr<ICUPSListener>;

    virtual ~ICUPSListener() {}
};

ICUPSListener::UPtrT createCupsListener(
    const onPrinterAdded& aPrinterAddedCallback,
    const onJobAdded& aJobAddedCallback);