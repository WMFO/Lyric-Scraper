package main

import (
	"fmt"
	"os"
	"strconv"
)

const VERSION = 1
const UNKNOWN = "UNKNOWN    ."
const EXPLICIT = "EXPLICIT   ."
const SAFE = "SAFE       ."
const USER = ""
const PASSWORD = ""

func main() {
	file, err := os.Open("regexfile.txt")

	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s\n", "regexfile.txt")
		os.Exit(1)
	}

	loadPatterns(file)

	net, err := os.OpenFile("networkErrors.log", os.O_WRONLY|os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "networkErrors.log", err.Error())
		os.Exit(1)
	}
	song, err := os.OpenFile("songClassification.log", os.O_WRONLY|os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "songClassification.log", err.Error())
		os.Exit(1)
	}
	dirty, err := os.OpenFile("dirtySongs.log", os.O_WRONLY|os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "dirtySongs.log", err.Error())
		os.Exit(1)
	}
	regex, err := os.OpenFile("regex.log", os.O_WRONLY|os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "regex.log", err.Error())
		os.Exit(1)
	}

	initLogging(net, song, dirty, regex)

	err = connect(USER, PASSWORD, "Rivendell")

	if err != nil {
		networkErrors.Printf("SQL ERROR: %s", err.Error())
		os.Exit(2)
	}

	s := os.Args[1]
	var n int
	if s == "all" {
		n = -1
	} else {
		n, err = strconv.Atoi(s)
		if err != nil {
			networkErrors.Printf("Bad command line args, exiting, %s", err.Error())
			os.Exit(1)
		}
	}
	checkNSongs(n)
}

func checkNSongs(n int) {

	// Get n songs
	rows := search(n)

	// If the search failed
	// Don't log the error here - it will have been logged in the mysql module
	if rows == nil {
		return
	}

	for rows.Next() {
		var number int
		var artist, title, album string
		err := rows.Scan(&number, &artist, &title, &album)
		dirty, err := checkAllSites(title, artist)

		var code string
		var log string
		if err != nil {
			networkErrors.Printf("NETWORK ERROR: %s", err.Error())
			code = UNKNOWN
			log = "NOT FOUND"
		} else if dirty {
			code = EXPLICIT
			log = "DIRTY"
		} else {
			code = SAFE
			log = "CLEAN"
		}
		q := fmt.Sprintf("UPDATE CART SET NOTES='flaggedBy=Scraper%d', SCHED_CODES='%s' WHERE NUMBER='%d'", VERSION, code, number)
		fmt.Printf("%s\n", q)
		err = query(q)
		if err != nil {
			networkErrors.Printf("SQL ERROR: %s", err.Error())
		}
		songClass.Printf("%s: Title: %s Artist: %s (id %d)\n", log, title, artist, number)
	}
}
