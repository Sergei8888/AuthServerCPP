//
// Created by Sergei Kuzmenkov on 20.04.2023.
//
#include <fstream>

#ifndef LAB_25_LOGGERTRANSPORT_H
#define LAB_25_LOGGERTRANSPORT_H

struct LoggerTransport {
    enum class LoggerTransportTypes {
        CONSOLE,
        FILE
    };
    
    std::ostream *outputStream;
    LoggerTransportTypes type;
};

#endif //LAB_25_LOGGERTRANSPORT_H
