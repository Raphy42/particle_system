//
// Created by Raphaël Dantzer on 12/10/16.
//

#ifndef PARTICLE_SYSTEM_FILELOGGER_H
#define PARTICLE_SYSTEM_FILELOGGER_H

#include <iostream>
#include <fstream>

#define FLOG_INFO(x)        (FileLogger::Instance() << FileLogger::e_logType::LOG_INFO << (x))
#define FLOG_WARNING(x)     (FileLogger::Instance() << FileLogger::e_logType::LOG_WARNING << (x))
#define FLOG_ERROR(x)       (FileLogger::Instance() << FileLogger::e_logType::LOG_ERROR << (x))
#define FLOG_CRITICAL(x)    (FileLogger::Instance() << FileLogger::e_logType::LOG_CRITICAL << (x))

class FileLogger
{
public:
    static FileLogger &Instance();

    FileLogger(std::string filename);
    FileLogger();

    ~FileLogger();

    enum class e_logType {
        LOG_CRITICAL,
        LOG_ERROR,
        LOG_WARNING,
        LOG_INFO
    };

    e_logType getSeverity() const;

    friend FileLogger &operator << (FileLogger &logger, const e_logType type);

    template <typename T>
    friend FileLogger &operator << (FileLogger &logger, const T str) {
        logger._out << str << std::endl;
        if (logger.isNeedToQuit()) {
            logger._out << "Error threshold raised";
            exit(EXIT_FAILURE);
        }
        return logger;
    }

    void setSeverity(e_logType severity);
    bool isNeedToQuit() const;
    void setNeedToQuit(bool needToQuit);

protected:
    const std::ofstream &getOut() const;

private:
    std::ofstream                   _out;
    unsigned int                    _errorCount;
    unsigned int                    _warningCount;
    e_logType                       _severity;
    bool                            _needToQuit;
    static FileLogger               *_instance;
};

#endif //PARTICLE_SYSTEM_FILELOGGER_H
