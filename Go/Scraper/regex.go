package main

import (
	"bytes"
	"io"
	"regexp"
	"strings"
    "fmt"
)

var patterns []string

// Checks against each regex in the patterns array
// and returns true if any of them matched
func dirty(lyrics string) bool {
	for _, p := range patterns {
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
	return false
}

// Load patterns line by line into the patterns slice
func loadPatterns(r io.Reader) error {
	buf := new(bytes.Buffer)
	_, err := buf.ReadFrom(r)

	if err != nil {
		return err
	}

	patterns = strings.Split(buf.String(), "\n")
	return nil
}
