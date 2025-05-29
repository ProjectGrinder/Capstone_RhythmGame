#include <utils.h>

namespace Utils {
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
}
