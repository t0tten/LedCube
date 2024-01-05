#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>

enum LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Log {
protected:
    Log();
    bool dbg, inf, wrn, err;
    static Log* log;

public:
    void setLogLevel(LogLevel logLevel);
    void debug (std::string mesg);
    void info (std::string mesg);
    void warn (std::string mesg);
    void error (std::string mesg);
    static Log* GetInstance();
};

#endif //LOG_H