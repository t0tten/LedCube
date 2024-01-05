#include "Log.h"

Log* Log::log = nullptr;
Log::Log() {
    this->dbg = false;
    this->inf = false;
    this->wrn = false;
    this->err = false;
}

void Log::setLogLevel(LogLevel logLevel)
{
    switch(logLevel)
    {
        case DEBUG:
            dbg = true;
        case INFO:
            inf = true;
        case WARN:
            wrn = true;
        case ERROR:
            err = true;
    }
}
void Log::debug (std::string mesg)
{
    if (dbg)
    {
        std::cout << "DEBUG: " << mesg << std::endl;
    }
}
void Log::info (std::string mesg)
{
    if (inf)
    {
        std::cout << "INFO: "  << mesg << std::endl;
    }
}
void Log::warn (std::string mesg)
{
    if (wrn)
    {
        std::cout << "WARN: "  << mesg << std::endl;
    }
}
void Log::error (std::string mesg)
{
    if (err)
    {
        std::cout << "ERROR: "  << mesg << std::endl;
    }
}

Log* Log::GetInstance()
{
    if (log == nullptr)
    {
        log = new Log();
    }
    return log;
}