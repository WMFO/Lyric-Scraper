//
// Lyric Scraper v1
// grabs HTML of associated AZLyrics page for parsing using cURL
// g++ -Wall AZScraper.cpp -lcurl
//

#include "Lyrics.h"

size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp);
string curl_lyrics(string url);

string AZlyrics(string song, string band);
// More to come

string lyrics (string song, string band) {
    int one = 1;
    switch(one){
        case 1:
            return AZlyrics(song, band);
        default:
            return "";
    }
}

string AZlyrics(string song, string band){
    string url = "http://www.azlyrics.com/lyrics/" + band + "/" + song + ".html";
    string lyrics = curl_lyrics(url);
    int start = lyrics.find("<!-- start of lyrics -->") + 25;
    int end   = lyrics.find("<!-- end of lyrics -->");
    transform(lyrics.begin(), lyrics.end(), lyrics.begin(), ::tolower);
    return lyrics.substr(start, end-start);
}


size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp) {
    ostringstream *stream = (ostringstream*) userp;
    size_t count = size *nmemb;
    stream->write(buffer, count);
    return count;
}

string curl_lyrics(string url){
    if (DEBUG) cout << url << endl;
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
    else return "";
}
