#ifndef WMFOLYRICS
#define WMFOLYRICS

#include <curl/curl.h>
#include <cctype>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#define DEBUG 0

using namespace std;

string lyrics (string song, string band, int site);

#endif
