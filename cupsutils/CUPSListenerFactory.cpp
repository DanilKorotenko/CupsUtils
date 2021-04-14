#include "ICUPSListener.hpp"
#include "CUPSListenerImplOSX.hpp"

namespace CupsUtilities
{
    ICUPSListener::UPtrT createCupsListener()
    {
        return std::unique_ptr<CUPSListenerImplOSX>(new CUPSListenerImplOSX());
    }
}
