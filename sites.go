package main

import (
	"bytes"
	"encoding/xml"
	"errors"
	"fmt"
	"net/http"
	"net/url"
	"strings"
)

var (
	// Per site
	CONNECTION = errors.New("connection")
	SEARCH     = errors.New("search")
	FORMAT     = errors.New("format") // Ill-formatted html
	// Per search
	NO_SUCCESS = errors.New("nosuccess")
)

var sites []*site

// Check each site until one succeeds to download
// and check the lyrics. Return the first result 
// from a successful connection and search
// If all fail, return that none succeeded
func checkAllSites(song, artist string) (bool, error) {
	for _, s := range sites {
		dirty, err := s.check(song, artist)
		if err == nil {
			return dirty, nil
		} else {
			switch err {
			case CONNECTION:
				networkErrors.Printf("%s - title: %s artist: %s - CONNECTION FAILED", s.name, song, artist)
			case SEARCH:
				networkErrors.Printf("%s - title: %s artist: %s - SEARCH FAILED", s.name, song, artist)
			case FORMAT:
				networkErrors.Printf("%s - title: %s artist: %s - ILL-FORMATTED PAGE", s.name, song, artist)
			case NO_SUCCESS:
				panic("s.check() returned NO_SUCCESS - this should never happen")
			}
		}
	}
	return false, NO_SUCCESS
}

// In go, any function named "init" is called before main
// This initializes the slice of sites
func init() {
	sites = make([]*site, 3)
	sites[0] = new(site)
	sites[0].name = "AZLyrics"
	sites[0].f = AZLyrics

	sites[1] = new(site)
	sites[1].name = "LyricsCom"
	sites[1].f = LyricsCom

	sites[2] = new(site)
	sites[2].name = "ChartLyrics"
	sites[2].f = ChartLyrics
}

func AZLyrics(song, artist string) (bool, error) {
	scrsong := scrub(song, false)
	scrartist := scrub(artist, false)
	scrsong = removeThe(scrsong)
	site := "AZlyrics"
	url := fmt.Sprintf("http://www.azlyrics.com/lyrics/%s/%s.html", scrartist, scrsong)
	resp, err := http.Get(url)
	if err != nil {
		return false, err
	}

	buf := new(bytes.Buffer)
	buf.ReadFrom(resp.Body)
	lyrics := buf.String()

	strs := strings.Split(lyrics, "<!-- start of lyrics -->")
	if len(strs) < 2 {
		return false, SEARCH
	}

	strs = strings.Split(strs[1], "<!-- end of lyrics -->")
	if len(strs) < 2 {
		return false, FORMAT
	}

	return dirty(strings.ToLower(strs[0]), song, artist, site), nil
}

func LyricsCom(song, artist string) (bool, error) {
	scrsong := scrub(song, true)
	scrartist := scrub(artist, true)
	scrsong = removeThe(scrsong)
	site := "Lyrics.com"
	url := fmt.Sprintf("http://www.lyrics.com/%s-lyrics-%s.html", scrsong, scrartist)
	resp, err := http.Get(url)
	if err != nil {
		return false, err
	}

	buf := new(bytes.Buffer)
	buf.ReadFrom(resp.Body)
	lyrics := buf.String()

	strs := strings.Split(lyrics, "<!-- CURRENT LYRIC -->")
	if len(strs) < 2 {
		return false, SEARCH
	}

	strs = strings.Split(strs[1], "---")
	if len(strs) < 2 {
		return false, FORMAT
	}

	return dirty(strings.ToLower(strs[0]), song, artist, site), nil
}

type Result struct {
	LyricChecksum string
	LyricId       string
	Lyric         string
	Artist        string
	Song          string
}

type CLyrics struct {
	SearchLyricResult []Result
}

func ChartLyrics(song, artist string) (bool, error) {
	fmt.Printf("got to chartLyrics at all...\n")
	scrsong := url.QueryEscape(song)
	scrartist := url.QueryEscape(artist)
	site := "chartlyrics.com"
	searchurl := fmt.Sprintf("http://api.chartlyrics.com/apiv1.asmx/SearchLyric?artist=%s&song=%s", scrartist, scrsong)
	resp, err := http.Get(searchurl)
	if err != nil {
		fmt.Printf("error in first chartlyrics query: %+v\n", err)
		return false, err
	}

	v := CLyrics{}
	buf := new(bytes.Buffer)
	buf.ReadFrom(resp.Body)
	err = xml.Unmarshal([]byte(buf.String()), &v)
	match := Result{}
	if err != nil {
		fmt.Printf("error in parsing first chartlyrics response: %+v\n", err)
		return false, err
	}
	for i := 0; i < len(v.SearchLyricResult); i++ {
		if v.SearchLyricResult[i].Song == song && v.SearchLyricResult[i].Artist == artist {
			fmt.Printf("entire searchlyricresult: %+v\n", v.SearchLyricResult[i])
			match.LyricId = v.SearchLyricResult[i].LyricId
			match.LyricChecksum = v.SearchLyricResult[i].LyricChecksum
			match.Artist = v.SearchLyricResult[i].Artist
			match.Song = v.SearchLyricResult[i].Song
			break
		}
	}
	fmt.Printf("match things\n LyricId:%s\n LyricCheckSum:%s\n Song:%s\n Artist:%s\n", match.LyricId, match.LyricChecksum, match.Song, match.Artist)
	if match.LyricId == "" {
		fmt.Printf("nothing in the results matched...\n")
		return false, SEARCH
	}
	lyricsurl := fmt.Sprintf("http://api.chartlyrics.com/apiv1.asmx/GetLyric?lyricId=%s&lyricCheckSum=%s",
		match.LyricId, match.LyricChecksum)
	resp, err = http.Get(lyricsurl)
	if err != nil {
		fmt.Printf("error in second chartlyrics response: %+v\n", err)
		return false, err
	}
	buf.ReadFrom(resp.Body)
	err = xml.Unmarshal([]byte(buf.String()), &match)
	if err != nil {
		fmt.Printf("error in second xml parsing: %+v\n", err)
		return false, err
	}

	if match.Lyric == "" {
		return false, SEARCH
	}

	fmt.Printf("result from chartlyrics: %s\n", match.Lyric)

	return dirty(strings.ToLower(match.Lyric), song, artist, site), nil
}
