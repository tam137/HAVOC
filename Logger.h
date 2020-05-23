#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>

#include "en_LogLvl.h"
#include "string.h"


/**
 * A basic logger
 */
class Logger {

private:
    Logger();
    ~Logger() {}
    Logger(const Logger&);
    Logger & operator = (const Logger &);
    LogLvl logLvl = LogLvl::disabled;

public:
    static Logger& instance();
    void setLogLvl(LogLvl logLvl);
    void write(LogLvl logLvl, std::string msg);
    void trunc();

};

#endif // LOGGER