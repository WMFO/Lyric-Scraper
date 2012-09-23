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

using namespace std;

string scrub_str (string str);
string dash_str (string str);
string scrape (string url);

int main (int argc, char *argv[]) {
    assert(argc > 2);
    //string song = scrub_str(argv[1]);
    //string band = scrub_str(argv[2]);
    string song = argv[1];
    string band = argv[2];

    if (OUTPUT) cout << "Song: " << song << endl;
    if (OUTPUT) cout << "Band: " << band << endl;

    //Some sites might want the "the"; consider moving the site-specific functions
    if (band.compare(0, 3, "the") == 0) {
        band.erase(0, 3);
    }

    if (OUTPUT) cout << lyrics(scrub_str(song), scrub_str(band), 1) << endl;
    if (OUTPUT) cout << lyrics(dash_str(song), dash_str(band), 2) << endl;

    return 0;
}

string scrub_str (string str){
    for (uint i=0; i<str.length(); i++) {
        if (!isalnum(str[i])) { 
            str.erase(i, 1);
        }
    }
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

string dash_str (string str){
    for (uint i=0; i<str.length(); i++) {
        if(isspace(str.at(i))) {
            str[i] = '-';
        }
    }
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

