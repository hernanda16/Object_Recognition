#ifndef __LOGGER_H_
#define __LOGGER_H_

#include <cstring>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <time.h>
#include <unistd.h>

/**
 * @brief This package will be the header for logger
 * our logger could print with several color
 */

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define RST "\033[0m"

#define LOGDIR "~/catkin_ws/Log/log.txt"

void Logger(const char* color, const char* msg, ...);
void SaveLog(const char* msg, ...);

#endif