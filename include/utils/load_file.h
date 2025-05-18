#pragma once
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <system_error>

namespace Utils
{
    bool load_file(const std::string path, std::vector<uint8_t>& buffer)
    {
        // Obtain file size
        std::error_code ec;
        std::uint64_t size = std::filesystem::file_size(path, ec);
        if (ec)
        {
            // Some error occurred while getting file size
            return false;
        }

        // Open the file
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            // File could not be opened
            return false;
        }

		// Resize the buffer to the file size
		buffer.resize(size);

        if (!file.read(reinterpret_cast<char*>(buffer.data()), size))
        {
            // File could not be read
            return false;
        }

        return true;
    }
}