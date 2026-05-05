#pragma once

namespace Game::Menu
{
    template<typename T>
    void enter_game([[maybe_unused]] T &syscall)
    {
        constexpr auto ENTER = 0x0D;

        if (get_key_state(ENTER))
        {
            LOG_INFO("Enter pressed");
        }
    }
}