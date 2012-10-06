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
#include "Lyrics.h"
#include "Log.h"

#define OUTPUT 1
#define MIN_LYR_LEN 64

using namespace std;

Log logger;
ofstream logfile;
void log_normal(string);
void log_error (string);
void log_fatal (string);

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
    
    assert(argc > 2);
    string song = argv[1];
    string band = argv[2];

    if (DEBUG) cout << "Song: " << song << endl;
    if (DEBUG) cout << "Band: " << band << endl;

    int nsites = numSites();
    for (int i = 0; i < nsites; i++){
        string result = lyrics(song, band, i);
        if (result.length() > MIN_LYR_LEN){
            if (OUTPUT) cout << result << endl;
            break;
        }
    }

    delete logger;
    return 0;
}

void log_normal(string err) { cout    << err << endl; }
void log_error (string err) { logfile << err << endl; }
void log_fatal (string err) { logfile << err << endl; }