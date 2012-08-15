//
// Lyric Scraper v1
// grabs HTML of associated AZLyrics page for parsing using cURL
// g++ -Wall AZScraper.cpp -lcurl
//

#include <curl/curl.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#define DEBUG 1 

using namespace std;

size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp);
string build_url (string band, string song);
string scrape (string url);

int main () {
    string band;
    string song;
    cout << "Band: "<<endl;
    getline (cin, band);
    cout << "Song:" <<endl;
    getline (cin, song);
    if (DEBUG) cout << band << " " << song <<endl;

    string url = build_url(band, song);
    if (DEBUG) cout << url<<endl;

    string lyrics = scrape(url);
    if (DEBUG) cout << lyrics <<endl;

    return 0;
}

size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp) {
    ostringstream *stream = (ostringstream*) userp;
    size_t count = size *nmemb;
    stream->write(buffer, count);
    return count;
}

string build_url (string band, string song) {
    for (uint i=0; i<band.length(); i++) {
        if (!isalnum(band[i])) { 
            band.erase(i, 1);
        }
    }
    for (uint j=0; j<song.length(); j++) {
        if (!isalnum(song[j])) {
            song.erase(j, 1);
        }
    }
    transform(song.begin(), song.end(), song.begin(), ::tolower);
    transform(band.begin(), band.end(), band.begin(), ::tolower);
    if (band.compare(0, 3, "the") == 0) {
        band.erase(0, 3);
    }
    return "http://www.azlyrics.com/lyrics/" + band + "/" + song +
    ".html";
} 

string scrape (string url) {
    CURL *handle;
    CURLcode error;
    ostringstream buffer;
    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_VERBOSE, DEBUG);
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(handle, CURLOPT_FAILONERROR, true);
    
    error = curl_easy_perform(handle);
    curl_easy_cleanup(handle);

    if (!error) return buffer.str();
    else return "Error!";
}
