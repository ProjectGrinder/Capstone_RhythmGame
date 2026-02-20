#include "system.h"

#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *IntentStorageHandler;
    extern "C" IntentStorageHandler get_render_storage();

    IntentStorage &IntentStorage::instance()
    {
        auto *instance = static_cast<IntentStorage *>(get_render_storage());
        if (instance == nullptr)
        {
            LOG_ERROR("IntentStorage used before initialization or after cleanup");
            std::abort();
        }
        return (*instance);
    }

    size_t IntentStorage::alloc_slot()
    {
        instance()._intent_storage.emplace_back(std::nullopt);
        return (instance()._intent_storage.size() - 1);
    }

    std::optional<DrawIntent> &IntentStorage::get_intent(const size_t slot)
    {
        return (instance()._intent_storage[slot]);
    }

    void IntentStorage::free_slot(const size_t slot)
    {
        instance()._intent_storage[slot] = std::nullopt;
    }

    Camera &IntentStorage::get_camera()
    {
        return (instance()._camera);
    }

} // namespace System
