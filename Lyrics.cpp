//
// Lyric Scraper v1
// grabs HTML of associated AZLyrics page for parsing using cURL
// g++ -Wall AZScraper.cpp -lcurl
//

#include "Lyrics.h"
using namespace std;

//Precondition: str is lowercase.
void removeThe(string str);

string scrub_str (string str);
string dash_str (string str);

size_t write_data(char *buffer, size_t size, size_t nmemb, void *userp);
string curl_lyrics(string url);

string AZlyrics(string song, string band);
string LyricsCom(string song, string band);
// More to come

Lyrics::Lyrics() {
    sites = vector<site>(2);
    sites[0].name = "AZlyrics";
    sites[0].f    = AZlyrics;
    
    sites[1].name = "LyricsCom";
    sites[1].f    = LyricsCom;
}
vector<string> Lyrics::lyrics(string song, string band) {
    vector<string> lyrics = vector<string>(sites.size());
    for (int i = 0; i < (int)sites.size(); i++) {
        lyrics[i] = sites[i].f(song, band);
    }
    return lyrics;
}
string Lyrics::lyrics(string song, string band, int site) {
    if (site < 0 || site >= (int)sites.size())
        return "";
    return sites[site].f(song, band);
}
int Lyrics::numSites() {
    return (int)sites.size();
}

string AZlyrics(string song, string band){
    song = scrub_str(song);
    band = scrub_str(band);
    removeThe(song);
    string url = "http://www.azlyrics.com/lyrics/" + band + "/" + song + ".html";
    string lyrics = curl_lyrics(url);
    
    // Handle errors
    if (lyrics.length() == 1 && lyrics[0] == ERROR_CHAR)
        return lyrics;
    
    int start = lyrics.find("<!-- start of lyrics -->") + 25;

    // find returns npos on no find. Check against '<' in case start == npos by accident
    if (start - 25 == lyrics.npos && lyrics[lyrics.npos] != '<') {
        lyrics = "a";
        lyrics[0] = NOT_FOUND;
        return lyrics;
    }
    
    int end   = lyrics.find("<!-- end of lyrics -->");
    transform(lyrics.begin(), lyrics.end(), lyrics.begin(), ::tolower);
    return lyrics.substr(start, end-start);
}

string LyricsCom(string song, string band){
    song = dash_str(song);
    band = dash_str(band);
    removeThe(song);
    string url = "http://www.lyrics.com/" + song + "-lyrics-" + band + ".html";
    string lyrics = curl_lyrics(url);
    
    // Handle errors
    if (lyrics.length() == 1 && lyrics[0] == ERROR_CHAR)
        return lyrics;
    
    int start = lyrics.find("<div id=\"lyrics\" class=\"SCREENONLY\">") + 36;
    
    // find returns npos on no find. Check against '<' in case start == npos by accident
    if (start - 36 == lyrics.npos && lyrics[lyrics.npos] != '<') {
        lyrics = "a";
        lyrics[0] = NOT_FOUND;
        return lyrics;
    }
    
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

// Returns a length-1 string whose character code is ERROR_CHAR upon error
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
    
    if (!error)
        return buffer.str();
    string out = "a";
    out[0] = ERROR_CHAR;
    return out;
}
