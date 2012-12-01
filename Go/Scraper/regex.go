package main

import (
	"bytes"
	"io"
	"regexp"
	"strings"
)

var patterns []string
var regexps *[]regexp.Regexp

// Checks against each regex in the patterns array
// and returns true if any of them matched
func dirty(lyrics string) bool {
	for _, r := range regexps {
		match := r.FindString(lyrics)
		if match != "" {
			dirtySong.Printf("Bad word: %s matches regexp: %s in song: %s by artist: %s", match, r.String(), "N/A", "N/A")
			return true
		}
	}
	return false

	/*for _, p := range patterns {
	        if(p == ""){
	            continue
	        }
			match, err := regexp.MatchString(p, lyrics)
			if err != nil {
				regexErrors.Printf("REGEX: %s\nLYRICS: %s", p, lyrics)
			} else if match {
	            fmt.Printf("Found match %s\n", p)
				return true
			}
		}
		return false*/
}

// Create proper regex type
func loadPatterns(r io.Reader) error {
	buf := new(bytes.Buffer)
	_, err := buf.ReadFrom(r)

	if err != nil {
		return err
	}

	patterns = strings.Split(buf.String(), "\n")

	for i, p := range patterns {
		if p == "" {
			continue
		}
		regexps[i], _ = Compile(p)
	}
	return nil
}

// Load patterns line by line into the patterns slice
/*func loadPatterns(r io.Reader) error {
	buf := new(bytes.Buffer)
	_, err := buf.ReadFrom(r)

	if err != nil {
		return err
	}

	patterns = strings.Split(buf.String(), "\n")
	return nil
}*/
