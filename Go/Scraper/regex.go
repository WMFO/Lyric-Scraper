package main

import (
	"bytes"
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
		} else if match {
			return true
		}
	}
	return false
}

func loadPatterns(r io.Reader) error {
	buf := new(bytes.Buffer)
	_, err := buf.ReadFrom(r)

	if err != nil {
		return err
	}

	patterns = strings.Split(buf.String(), "\n")
	return nil
}
