#pragma once

#include <string>
#include <format>
#include <chrono>
#include <mutex>

namespace Utils
{
    static std::mutex debug_mutex;
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
        std::string file_path,
        std::string func_name,
        std::format_string<args...> format,
        args&&... argv
    )
    {
        std::lock_guard<std::mutex> lock(debug_mutex);

        const std::chrono::system_clock::time_point time_point_utc = std::chrono::system_clock::now();

        std::stringstream time_stream;
        time_stream << std::chrono::current_zone()->to_local(time_point_utc);
        const std::string time_str = time_stream.str();

        const std::string message = std::format(format, std::forward<args>(argv)...);
        const std::string log =
            std::format
            (
                "[ {} | {} | {} ] - {}""\n",
                time_str,
                file_path,
                func_name,
                message
            ).c_str();

        OutputDebugStringA
        (
            log.c_str()
        );
    }
}
