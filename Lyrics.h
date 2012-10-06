#ifndef WMFOLYRICS
#define WMFOLYRICS

#include <curl/curl.h>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define DEBUG 0
#define ERROR_CHAR 255

using namespace std;

class Lyrics {
public:
    Lyrics();
    
    // Returns a length-1 string whose character code is ERROR_CHAR upon curl error
    vector<string> lyrics(string song, string band);
    
    // Returns a length-1 string whose character code is ERROR_CHAR upon curl error
    string lyrics(string song, string band, int site);
    int numSites();
private:
    typedef string(*fetch)(string,string);
    struct site {
        fetch f;
        string name;
    };
    
    vector<site> sites;
};

#endif
