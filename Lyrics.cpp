//
// Lyric Scraper v1
// grabs HTML of associated AZLyrics page for parsing using cURL
// g++ -Wall AZScraper.cpp -lcurl
//

#include "Lyrics.h"

//Precondition: str is lowercase.
void removeThe(string str);

string scrub_str (string str);
string dash_str (string str);

size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp);
string curl_lyrics(string url);

string AZlyrics(string song, string band);
string LyricsCom(string song, string band);
// More to come

int numSites(){
    return 3; //MUST match the case statement in the next function
}

string lyrics (string song, string band, int site) {
    switch(site){
        case 0:
            return ""; //reserved
        case 1:
            return AZlyrics(song, band);
        case 2:
            return LyricsCom(song, band);
        default:
            return "";
    }
}

string AZlyrics(string song, string band){
    song = scrub_str(song);
    band = scrub_str(band);
    removeThe(song);
    string url = "http://www.azlyrics.com/lyrics/" + band + "/" + song + ".html";
    string lyrics = curl_lyrics(url);
    int start = lyrics.find("<!-- start of lyrics -->") + 25;
    int end   = lyrics.find("<!-- end of lyrics -->");
    transform(lyrics.begin(), lyrics.end(), lyrics.begin(), ::tolower);
    return lyrics.substr(start, end-start);
}

string LyricsCom(string song, string band){
    song = dash_str(song);
    band = dash_str(band);
    removeThe(song);
    string url = "http://www.lyrics.com/" + song + "-lyrics-" + band + ".html";
    cout << url << endl;
    string lyrics = curl_lyrics(url);
    int start = lyrics.find("<!-- CURRENT LYRIC -->") + 22;
    int end   = lyrics.find("---");
    transform(lyrics.begin(), lyrics.end(), lyrics.begin(), ::tolower);
    return lyrics.substr(start, end-start);
}

void removeThe(string str){
    if (str.compare(0, 3, "the") == 0)
        str.erase(0, 3);
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
