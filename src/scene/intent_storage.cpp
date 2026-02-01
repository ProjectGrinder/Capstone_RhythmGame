#include "system.h"

#include "utils/print_debug.h"

namespace System
{
    typedef void *IntentStorageHandler;
    extern "C" IntentStorageHandler get_intent_storage();

    IntentStorage &IntentStorage::instance()
    {
        auto *instance = static_cast<IntentStorage *>(get_intent_storage());
        if (instance == nullptr)
        {
            LOG_ERROR("IntentStorage used before initialization or after cleanup");
            std::abort();
        }
        return (*instance);
    }

    size_t IntentStorage::alloc_slot()
    {
        instance()._storage.emplace_back(std::nullopt);
        return (instance()._storage.size() - 1);
    }

    void IntentStorage::free_slot(const size_t slot)
    {
        instance()._storage[slot] = std::nullopt;
    }

}