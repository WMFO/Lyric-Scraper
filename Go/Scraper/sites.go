package main

import (
	"bytes"
	"errors"
	"fmt"
	"net/http"
	"strings"
)

var (
	// Per site
	CONNECTION = errors.New("connection")
	SEARCH     = errors.New("serach")
	FORMAT     = errors.New("format") // Ill-formatted html
	// Per search
	NO_SUCCESS = errors.New("nosuccess")
)

var sites []*site

func checkAll(song, artist string) (bool, error) {
	for _, s := range sites {
		dirty, err := s.check(song, artist)
		if err == nil {
			// TODO: log error
			return dirty, nil
		}
	}
	return false, NO_SUCCESS
}

func init() {
	sites = make([]*site, 2)
	sites[0].name = "AZLyrics"
	sites[0].f = AZLyrics

	sites[1].name = "LyricsCom"
	sites[1].f = LyricsCom
}

func AZLyrics(song, artist string) (bool, error) {
	song = scrub(song, false)
	artist = scrub(artist, false)
	song = removeThe(song)
	url := fmt.Sprintf("http://www.azlyrics.com/lyrics/%s/%s.html", artist, song)
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

	return dirty(strings.ToLower(strs[0])), nil
}

func LyricsCom(song, artist string) (bool, error) {
	song = scrub(song, true)
	artist = scrub(artist, true)
	song = removeThe(song)
	url := fmt.Sprintf("http://www.lyrics.com/%s-lyrics-%s.html", song, artist)
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

	return dirty(strings.ToLower(strs[0])), nil
}
