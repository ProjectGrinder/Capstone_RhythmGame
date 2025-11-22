#pragma once

namespace Game::Render
{
    struct Animation
    {
        unsigned int id[32];
        template <typename ... IDs>
        explicit Animation(IDs ...ids) : id{}
        {
            constexpr int n = sizeof...(IDs);
            const unsigned int tmp[n] = { static_cast<unsigned int>(ids)...};
            std::memcpy(id, tmp, sizeof(tmp));
            if constexpr (n < 32)
            {
                for (int i = n; i < 32; ++i)
                {
                    id[i] = 0;
                }
            }
        }
    };
}