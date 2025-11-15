#include <algorithm>
#include <utils.h>

namespace Utils {
    std::mutex log_mutex;

    std::string get_local_time_string()
    {
        SYSTEMTIME st;
        GetLocalTime(&st);
        std::string format = std::format
            (
                "{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}",
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
            );
        return(format);
    }

    std::string normalize_path(std::string_view path)
    {
        std::string result(path);
        std::ranges::replace(result, '\\', '/');
        return(result);
    }
}
