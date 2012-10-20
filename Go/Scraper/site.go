package main

import (
	"strings"
)

type site struct {
	name string
	f    func(string, string) (bool, error)
}

// Returns true for dirty, false for clean
func (s *site) check(song, artist string) (bool, error) {
	return s.f(song, artist)
}

func scrub(str string) string {
	str = strings.ToLower(str)
	out := make([]byte, len(str))
	i := 0
	j := 0
	for ; i < len(str); i++ {
		c := str[i]
		if (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') {
			out[j] = str[i]
			j++
		}
	}
	return string(out[:j])
}

func removeThe(str string) string {
	return strings.Join(strings.Split(str, "the"), "")
}
