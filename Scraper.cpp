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
#include "Regex.h"

#define MIN_LYR_LEN 64
#define REGEXFILE "filter.txt"

// Exit codes
#define GOOD 0
#define USAGE 1
#define CONNECTION 2
#define NOT_FOUND 3
#define BOTH 4
#define DIRTY 5


using namespace std;

Log logger;
ofstream logfile;
void log_normal(string);
void log_error (string);
void log_fatal (string);
string timestamp();

void postRegex(string, string, string, string);

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
    
    if (argc != 3) {
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
    int errors = 0;
    int not_found = 0;
    for (int i = 0; i < l.numSites(); i++) {
        string lyric = l.lyrics(song, band, i);
        if (lyric.length() > MIN_LYR_LEN) {
            lyric = regex(lyric);
            postRegex(lyric, song, band, l.getName(i));
            break;
        } else if (lyric.length() == 1) {
            if (lyric[0] == ERROR_CHAR) {
                errors++;
                logger.log("Could not connect to " + l.getName(i) + ".", LOG_ERROR);
            }
            else if (lyric[0] == NOT_FOUND_CHAR) {
                // logger.log("Could not find " + song + " by " + band + " at " + l.getName(i), LOG_NORMAL);
                not_found++;
            }
        }
    }
    
    // If all curls failed, there was a connection error
    if (errors == l.numSites()) {
        logger.log("Could not make any connections.", LOG_ERROR);
        return CONNECTION;
    }
    // If all searches faild, report the song as not found
    if (not_found == l.numSites()) {
        logger.log("Could not find " + song + " by " + band + " on any sites.", LOG_ERROR);
        return NOT_FOUND;
    }
    
    logger.log("Some searches failed and some connections failed; none succeeded.", LOG_ERROR);
    return BOTH;
}

void postRegex(string lyric, string song, string band, string site) {
    if (lyric.length() < 1)
        exit(GOOD);
    logger.log("Found bad language in " + song + " by " + band + " at " + site + ": " + lyric, LOG_ERROR);
    exit(DIRTY);
}

void log_normal(string err) { 
    cout << err << endl;
}
void log_error (string err) { 
    logfile << timestamp() << ": " << err << endl;
    cerr << err << endl;
}
void log_fatal (string err) { 
    logfile << timestamp() << ": " << err << endl;
    cerr << err << endl;
}

string timestamp(){
    time_t rawtime;
    time (&rawtime);
    return ctime(&rawtime);
}