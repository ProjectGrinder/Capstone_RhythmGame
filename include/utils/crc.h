#pragma once
#include <cstdint>
#include <vector>

namespace Utils
{
    std::array<uint32_t, 256> _crc_table;
    bool _crc_table_initialized = false;

    void _build_crc_table() {
        std::for_each
        (
            _crc_table.begin(), 
            _crc_table.end(), 
            [index = 0u](uint32_t& crc) mutable 
            {
                crc = index;
                for (int k = 0; k < 8; k++) {
                    crc = (crc & 1) ? (0xEDB88320U ^ (crc >> 1)) : (crc >> 1);
            }
            ++index;
        });
    }

    uint32_t generate_crc(std::vector<uint8_t> message)
    {
        uint8_t* data = message.data();
        size_t length = message.size();
        if (!_crc_table_initialized) {
            _build_crc_table();
            _crc_table_initialized = true;
        }

        uint32_t crc = 0xFFFFFFFF;
        std::for_each
        (
            message.begin(), 
            message.end(), 
            [&crc](uint8_t byte) 
            {
                crc = (crc >> 8) ^ _crc_table[(crc ^ byte) & 0xFF];
            }
        );
        return crc ^ 0xFFFFFFFF;
    }
}