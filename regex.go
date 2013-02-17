package main

import (
	"bytes"
	"fmt"
	"io"
	"regexp"
	"strings"
)

var patterns []string
var expressions []*regexp.Regexp

// Checks against each regex in the patterns array
// and returns true if any of them matched
func dirty(lyrics, song, artist, site string) bool {
	for _, r := range expressions {
		match := r.FindString(lyrics)
		if match != "" {
			dirtySong.Printf("Bad word: %s matches regexp: %s in song: %s by artist: %s from site: %s", match, r.String(), song, artist, site)
			return true
		}
	}
	return false
}

// Create proper regex type
func loadPatterns(r io.Reader) error {
	buf := new(bytes.Buffer)
	_, err := buf.ReadFrom(r)

	if err != nil {
		return err
	}

	patterns = strings.Split(buf.String(), "\n")

	expressions = make([]*regexp.Regexp, 0, len(patterns))
	var temp *regexp.Regexp

	for _, p := range patterns {
		if p == "" {
			continue
		}
		temp, err = regexp.Compile(p)
		if err != nil {
			fmt.Printf("%s pattern failed to compile\n", p)
			continue
		}
		expressions = append(expressions, temp)
	}
	return nil
}
