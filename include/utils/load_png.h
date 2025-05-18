#pragma once
#include <filesystem>
#include <cstdint>
#include <vector>
#include "utils/load_file.h"

// Courtesy of https://evanhahn.com/worlds-smallest-png/
#define SMALLEST_PNG_SIZE 67
#define PNG_SIGNATURE_SIZE 8
#define PNG_CHUNK_LENGTH_SIZE 4
#define PNG_CHUNK_TYPE_SIZE 4
#define PNG_CHUNK_CRC_SIZE 4

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

    enum _png_parse_state
    {
        VERIFY_HEADER,
        READ_CHUNK_LENGTH,
        READ_CHUNK_TYPE,
        READ_CHUNK_DATA,
        READ_CHUNK_CRC
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

        _png_parse_state state = VERIFY_HEADER;

        std::vector<uint8_t> png_signature = 
        {
            0x89, 0x50, 0x4E, 0x47,
            0x0D, 0x0A, 0x1A, 0x0A
        };

        uint32_t counter = 0;
        uint32_t length = 0;
        uint32_t chunk_type = 0;
        uint32_t chunk_crc = 0;
        std::vector<uint8_t> chunk_data;

        for (auto byte : buffer)
        {
            switch (state)
            {
            case VERIFY_HEADER:
                if (byte != png_signature[counter])
                {
                    return false;
                }
                counter++;
                if (counter == png_signature.size())
                {
                    state = READ_CHUNK_LENGTH;
                    counter = 0;
                    length = 0;
                }
                break;
            case READ_CHUNK_LENGTH:
                length = (length << 8) | byte;
                counter++;
                if (counter == PNG_CHUNK_LENGTH_SIZE)
                {
                    state = READ_CHUNK_TYPE;
                    counter = 0;
                    chunk_data.resize(length+4);
                }
                break;
            case READ_CHUNK_TYPE:
                chunk_type = (chunk_type << 8) | byte;
				chunk_data[counter] = byte;
                counter++;
                if (counter == PNG_CHUNK_TYPE_SIZE)
                {
                    state = READ_CHUNK_DATA;
                    counter = 0;

                }
                break;
            case READ_CHUNK_DATA:
                chunk_data[counter+4] = byte;
                counter++;
                if (counter == length)
                {
                    state = READ_CHUNK_CRC;
                    counter = 0;
                }
                break;
            case READ_CHUNK_CRC:
                chunk_crc = (chunk_crc << 8) | byte;
                counter++;
                if (counter == PNG_CHUNK_CRC_SIZE)
                {
                    state = VERIFY_HEADER;
                    counter = 0;
                    //TODO: CRC check
                    //TODO: Process chunk data
                }
            }
        }

        if (state != VERIFY_HEADER)
        {
            return false;
        }
        if (counter != 0)
        {
            return false;
        }
        if (length != 0) // final IEND chunk has length 0
        {
            return false;
        }

        return true;
    }
}