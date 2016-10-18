//
// Created by Raphaël Dantzer on 12/10/16.
//

#include <ios>
#include <sstream>
#include <iomanip>
#include "FileLogger.h"

//Singleton instance
FileLogger      *FileLogger::_instance = nullptr;

static std::string getDate() {
//cpp11 only!
    time_t t = std::time(nullptr);
    tm *tm = std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(tm, "%d-%m-%Y %H-%M-%S");
    return std::string(oss.str());
}

FileLogger::FileLogger() :
        _errorCount(0U), _warningCount(0U), _severity(e_logType::LOG_CRITICAL), _needToQuit(false) {
    _out.open("particle_system.log");
    if (_out.is_open())
        _out << "Log started @ " << getDate() << std::endl;
    else
        throw std::runtime_error("Failed to init logging interface"); //TODO remove exception
}

FileLogger::FileLogger(std::string filename) :
        _errorCount(0U), _warningCount(0U), _severity(e_logType::LOG_CRITICAL), _needToQuit(false) {
    _out.open(filename.c_str());
    if (_out.is_open())
        _out << "Log started @ " << getDate()  << std::endl;
    else
        throw std::runtime_error("Failed to init logging interface"); //TODO remove exception
}

FileLogger::~FileLogger() {
    FLOG_INFO("Bye!");
    _out.close();
}

bool FileLogger::isNeedToQuit() const {
    return _needToQuit;
}

void FileLogger::setNeedToQuit(bool needToQuit) {
    _needToQuit = needToQuit;
}

FileLogger::e_logType FileLogger::getSeverity() const {
    return _severity;
}

void FileLogger::setSeverity(FileLogger::e_logType severity) {
    _severity = severity;
}

const std::ofstream &FileLogger::getOut() const {
    return _out;
}

FileLogger &operator << (FileLogger &logger, const FileLogger::e_logType type) {
    switch (type) {
        case FileLogger::e_logType::LOG_CRITICAL:
            logger._out << "[CRITICAL]: ";
            break ;
        case FileLogger::e_logType::LOG_ERROR:
            logger._out << "[ERROR]: ";
            ++logger._errorCount;
            break ;
        case FileLogger::e_logType::LOG_WARNING:
            logger._out << "[WARNING]: ";
            ++logger._warningCount;
            break ;
        case FileLogger::e_logType::LOG_INFO:
            logger._out << "[INFO]: ";
            break ;
    }
    if (logger.getSeverity() == type)
        logger.setNeedToQuit(true);
    return (logger);
}

FileLogger &FileLogger::Instance() {
    if (FileLogger::_instance == nullptr)
        FileLogger::_instance = new FileLogger;
    return (*FileLogger::_instance);

}
