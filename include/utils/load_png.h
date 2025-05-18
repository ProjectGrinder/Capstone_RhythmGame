#pragma once
#include <filesystem>
#include <cstdint>
#include <vector>
#include "utils/load_file.h"

// Courtesy of https://evanhahn.com/worlds-smallest-png/
#define SMALLEST_PNG_SIZE 67

namespace Utils
{
    /// <summary>
    /// Decoded PNG data structure extended or collapsed to RGBA8.
    /// </summary>
    /// <param name="data">Decoded pixel data</param>
    /// <param name="width">Width of the image</param>
    /// <param name="height">Height of the image</param>
    struct PngData
    {
        uint8_t* data;
        uint32_t width = 0;
        uint32_t height = 0;
        ~PngData() { delete[] data; }

        PngData(const PngData&) = delete;
        PngData& operator=(const PngData&) = delete;

        PngData(PngData&& other) noexcept
            : data(other.data), width(other.width), height(other.height)
        {
            other.data = nullptr;
            other.width = 0;
            other.height = 0;
        }

        PngData& operator=(PngData&& other) noexcept
        {
            if (this != &other) {
                delete[] data;
                data = other.data;
                width = other.width;
                height = other.height;

                other.data = nullptr;
                other.width = 0;
                other.height = 0;
            }
            return *this;
        }
    };

    /// <summary>
    /// Loads a PNG file from the specified path into memory.
    /// </summary>
    /// <param name="path">Path to the PNG file</param>
    /// <param name="data">Reference to a PngData structure to hold the loaded data</param>
    /// <returns>True if the PNG file was loaded successfully, false otherwise</returns>
    bool load_png(const std::string path, PngData& data)
    {
        std::vector<uint8_t> buffer;
        bool success = true;
        success = load_file(path, buffer);
        if (!success)
        {
            return false;
        }

        if (buffer.size() < SMALLEST_PNG_SIZE)
        {
            return false;
        }

        // Verify PNG Signature
        static const uint8_t png_signature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
        if (!std::equal(buffer.begin(), buffer.begin() + 8, png_signature))
        {
            return false;
        }

        // Chunking logic

    }
}