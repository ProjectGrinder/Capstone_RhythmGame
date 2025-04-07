#pragma once

#include <string>
#include <format>
#include <chrono>
#include <ctime>
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

        const std::chrono::time_point now = std::chrono::system_clock::now();
        const std::time_t c_time = std::chrono::system_clock::to_time_t(now);
        const std::string time_str = std::string(std::ctime(&c_time));
        const std::string trimmed_time = time_str.substr(0, time_str.length() - 1);

        const std::string message = std::format(format, std::forward<args>(argv)...);
        const std::string log =
            std::format
            (
                "[ {} | {} | {} ] - {}""\n",
                trimmed_time,
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
