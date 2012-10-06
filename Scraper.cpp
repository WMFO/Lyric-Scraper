//
// Lyric Scraper v1
// grabs HTML of associated AZLyrics page for parsing using cURL
// g++ -Wall AZScraper.cpp -lcurl


#include <curl/curl.h>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <assert.h>
#include <fstream>
#include <vector>
#include "Lyrics.h"
#include "Log.h"

#define OUTPUT 1
#define MIN_LYR_LEN 64

// Error codes
#define USAGE 1
#define NOTFOUND 2
#define CONNECTION 3


using namespace std;

Log logger;
ofstream logfile;
void log_normal(string);
void log_error (string);
void log_fatal (string);
string timestamp();

int main (int argc, char *argv[]) {
    
    string logfname = "logfile";
    
    logger = Log(log_normal, log_error, log_fatal);
    logfile.open(logfname.c_str());
    if (!logfile.is_open()) {
        cerr << "Could not open logfile: " << logfname << ". Continuing without logging..." << endl;
        // Normal logging goes to cout, so leave it be
        logger.setFunc(NULL, LOG_ERROR);
        logger.setFunc(NULL, LOG_FATAL);
    }
    
    if (argc != 2) {
        string argv0 = argv[0];
        logger.log("Usage: " + argv0 + " <song> <bands>", LOG_FATAL);
        exit(USAGE);
    }

    string song = argv[1];
    string band = argv[2];

    if (DEBUG) {
        logger.log("Song: " + song, LOG_NORMAL);
        logger.log("Band: " + band, LOG_NORMAL);
    }
    
    Lyrics l = Lyrics();
    vector<string> lyrics = l.lyrics(song, band);
    int errors = 0;
    for (int i = 0; i < (int)lyrics.size(); i++) {
        if (lyrics[i].length() > MIN_LYR_LEN) {
            logger.log(lyrics[i], LOG_NORMAL);
        } else if (lyrics[i].length() == 1 && lyrics[i][0] == ERROR_CHAR) {
            errors++;
        }
    }
    
    // If all curls failed, there was a connection error
    if (errors == (int)lyrics.size())
        return CONNECTION;
    
    return 0;
}

string timestamp(){
    time_t rawtime;
    time (&rawtime);
    return ctime(&rawtime);
}

void log_normal(string err) { 
    if (OUTPUT)
        cout << err << endl;
}
void log_error (string err) { 
    logfile << timestamp() << ": " << err << endl;
}
void log_fatal (string err) { 
    logfile << timestamp() << ": " << err << endl;
}