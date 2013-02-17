package main

import (
	"strings"
)

/*
	Each lyrics site is of type site, which allows us to loop over
	all sites quickly (yay, function pointers!)
*/

type site struct {
	name string
	f    func(string, string) (bool, error)
}

// Returns true for dirty, false for clean
func (s *site) check(song, artist string) (bool, error) {
	return s.f(song, artist)
}

/*
	If scrub is called with dashed, spaces are converted to dashes
	Otherwise they are simply omitted
	(ie, This Is A Song --> This-Is-A-Song or ThisIsASong)
*/
func scrub(str string, dashed bool) string {
	str = strings.ToLower(str)
	out := make([]byte, len(str))
	i := 0
	j := 0
	for ; i < len(str); i++ {
		c := str[i]
		if (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') {
			out[j] = str[i]
			j++
		} else if c == ' ' && dashed {
			out[j] = '-'
			j++
		}
	}
	return string(out[:j])
}

// Remove the word "the" from a string
func removeThe(str string) string {
	return strings.Join(strings.Split(str, "the"), "")
}
