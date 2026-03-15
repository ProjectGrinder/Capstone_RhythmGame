#include "system.h"

#include "utils/print_debug.h"

namespace System::Render
{
    typedef void *IntentStorageHandler;
    extern "C" IntentStorageHandler get_intent_storage(void);

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
        auto &storage = instance()._intent_storage;
        for (size_t i = 0; i < storage.size(); ++i)
        {
            if (storage[i].kind == DrawKind::KIND_UNKNOWN)
                return i;
        }
        storage.push_back(DrawIntent{});
        return storage.size() - 1;
    }

    void IntentStorage::free_slot(size_t slot)
    {
        instance()._intent_storage[slot].kind = DrawKind::KIND_UNKNOWN;
    }

    DrawIntent &IntentStorage::get_intent(size_t slot)
    {
        return instance()._intent_storage[slot];
    }

    void IntentStorage::next_frame()
    {
        auto &self = instance();
        self._current_frame_idx = (self._current_frame_idx + 1) % NUM_BUFFERS;
        self._buffer_counts[self._current_frame_idx] = 0;
    }

    DrawIntent &IntentStorage::allocate_packed()
    {
        auto &self = instance();
        auto &buf = self._packed_buffers[self._current_frame_idx];
        size_t &count = self._buffer_counts[self._current_frame_idx];

        if (count >= buf.size())
        {
            buf.push_back(DrawIntent{});
        }

        DrawIntent &ref = buf[count];
        ref.kind = DrawKind::KIND_UNKNOWN;
        ref.common.visible = true;

        count++;
        return ref;
    }

    const std::vector<DrawIntent> &IntentStorage::get_packed_for_render(uint32_t render_frame_idx)
    {
        auto &self = instance();
        auto &buf = self._packed_buffers[render_frame_idx];
        size_t count = self._buffer_counts[render_frame_idx];

        if (buf.size() > count)
        {
            buf.resize(count);
        }
        return buf;
    }

    const std::vector<DrawIntent> &IntentStorage::get_intents()
    {
        auto &self = instance();
        auto &data = get_packed_for_render(self._current_frame_idx);
        return (data);
    }

} // namespace System::Render
