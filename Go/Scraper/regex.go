package main

import (
	"io"
	"regexp"
	"strings"
)

var patterns []string

func dirty(lyrics string) bool {
	for _, p := range patterns {
		match, err := regexp.MatchString(p, lyrics)
		if err != nil {
			// TODO
			// Log error
		} else if regexp.MatchString(p, lyrics) {
			return true
		}
	}
	return false
}

func loadPatterns(r io.Reader) error {
	buf := new(bytes.Buffer)
	buf.ReadFrom(r)
	patterns := strings.Split(buf.String(), "\n")
}
