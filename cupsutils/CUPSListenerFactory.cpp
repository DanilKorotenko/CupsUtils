#include "ICUPSListener.hpp"
#include "CUPSListenerImplOSX.hpp"

namespace CupsUtilities
{

ICUPSListener::UPtrT createCupsListener(
    const onPrinterAdded& aPrinterAddedCallback,
    const onJobAdded& aJobAddedCallback)
{
    return std::unique_ptr<CUPSListenerImplOSX>(new CUPSListenerImplOSX(
        aPrinterAddedCallback, aJobAddedCallback));
}

}
