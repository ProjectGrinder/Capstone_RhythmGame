#pragma once

#include <Windows.h>
#include <string>
#include <format>
#include <chrono>

#define FILE_PATH Utils::trim_src_path(__FILE__)
#define LOG_DEBUG(fmt, ...) Utils::print_debug(FILE_PATH, __FUNCTION__, fmt, __VA_ARGS__)

namespace Utils
{
    constexpr std::string_view trim_src_path(std::string_view path)
    {
        constexpr std::string_view key = R"(\src\)";
        size_t pos = path.find(key);
        return((pos != std::string_view::npos) ?
                    path.substr(pos + key.size()) :
                    path);
    }

    /// <summary>
    /// Formatted Debug Printing
    /// </summary>
    /// <param name="file_path">file path</param>
    /// <param name="func_name">name of the function (please add where the definition from)</param>
    /// <param name="format">format print</param>
    /// <param name="argv">argument list</param>
    template<typename... args>
    void print_debug
    (
        std::string_view file_path,
        std::string_view func_name,
        std::format_string<args...> format,
        args&&... argv
    )
    {
        const auto time_point_utc = std::chrono::system_clock::now();
        static const auto *zone = std::chrono::current_zone();
        const auto time = zone->to_local(time_point_utc);

        const std::string message = std::format(format, std::forward<args>(argv)...);
        const std::string log =
            std::format
            (
                "[ {} | {} | {} ] - {}\n",
                time,
                file_path,
                func_name,
                message
            );

        OutputDebugStringA(log.c_str());
    }
}
