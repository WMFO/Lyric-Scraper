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
#include "Lyrics.h"

#define OUTPUT 1
#define MIN_LYR_LEN 64

using namespace std;

int main (int argc, char *argv[]) {
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

    return 0;
}

