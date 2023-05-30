//
// Created by Sergei Kuzmenkov on 10.04.2023.
//
#include <string>

#ifndef LAB_25_TIME_H
#define LAB_25_TIME_H

class TimeUtils {
public:
    // Get current date/time, format is DD.MM.YYYY HH:mm:ss
    static std::string currentDateTimeForFile() {
        time_t now = time(nullptr);
        struct tm tstruct{};
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%d.%m.%Y %X", &tstruct);

        return buf;
    }

    // Get current date/time, format is HH:mm:ss
    static std::string currentDateTimeForConsole() {
        time_t now = time(nullptr);
        struct tm tstruct{};
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%X", &tstruct);

        return buf;
    }
};

#endif //LAB_25_TIME_H