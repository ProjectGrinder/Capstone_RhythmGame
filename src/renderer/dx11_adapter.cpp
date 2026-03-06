#include "system.h"

#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *DX11AdapterHandler;
    extern "C" DX11AdapterHandler get_dx11_adapter();

    Dx11Adapter &Dx11Adapter::instance()
    {
        auto *instance = static_cast<Dx11Adapter *>(get_dx11_adapter());
        if (instance == nullptr)
        {
            LOG_ERROR("Dx11Adapter used before initialization or after cleanup");
            std::abort();
        }
        return (*instance);
    }
}