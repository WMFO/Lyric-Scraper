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

using namespace std;

//string lyrics (string song, string band, int site);
//int numSites();

class Lyrics {
public:
    Lyrics();
    vector<string> lyrics(string song, string band);
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
