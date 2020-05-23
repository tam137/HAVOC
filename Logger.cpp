#ifndef LOGGER_CPP_INCLUDED
#define LOGGER_CPP_INCLUDED

#include "Logger.h"

Logger& Logger::instance() {
    //static Logger* _instance = new Logger();
    //return *_instance;
}

void Logger::setLogLvl(LogLvl logLvl) {
    this->logLvl = logLvl;
}

void Logger::write(LogLvl logLvl, std::string msg) {
    if (logLvl <= Logger::logLvl) {
        std::ofstream myfile;
        myfile.open ("app.log", std::ofstream::app);
        myfile << msg;
        myfile << "\n";
        myfile.close();
    }
}

void Logger::trunc() {
    std::ofstream myfile;
    myfile.open ("app.log", std::ofstream::trunc);
    myfile.close();        
}

#endif // LOG