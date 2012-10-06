#include <iostream>

// Log levels
#define LOG_NORMAL 0
#define LOG_ERROR 1
#define LOG_DIRTY 2

using namespace std;

// Function signature for logger

#ifndef Lyric_Scraper_Log_h
#define Lyric_Scraper_Log_h

class Log {
    typedef void(*logger)(string err);
public:
    Log();
    // Initialize with functions to be called on the various logging levels
    Log(logger normal, logger error, logger fatal);
    
    // Set the function to be called on logging level, l
    // Return 1 upon success, else 0
    int setFunc(logger l, int level);
    
    // Log an error
    // Return 1 upon success, else 0
    int log(string err, int level);
private:
    logger normal, error, fatal;
};

#endif
