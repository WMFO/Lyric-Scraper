package main

import (
	"fmt"
	"os"
)

func main() {
	file, err := os.Open("regexfile.txt")

	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s\n", "regexfile.txt")
		os.Exit(1)
	}

	loadPatterns(file)

	net, err := os.OpenFile("networkErrors.log", os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "networkErrors.log", err.Error());
		os.Exit(1)
	}
	song, err := os.OpenFile("songClassification.log", os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "songClassification.log", err.Error());
		os.Exit(1)
	}
	dirty, err := os.OpenFile("dirtySongs.log", os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "dirtySongs.log", err.Error());
		os.Exit(1)
	}
	regex, err := os.OpenFile("regex.log", os.O_APPEND, 0660)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error: could not open %s: %s\n", "regex.log", err.Error());
		os.Exit(1)
	}

	initLogging(net, song, dirty, regex)

	err = connect("user", "pass", "Rivendell")
	
	if err != nil {
		fmt.Fprintf(os.Stderr, "SQL ERROR: %s\n", err.Error())
		networkErrors.Printf("SQL ERROR: %s", err.Error())
		os.Exit(2)
	}
	
	checkOneSong()
}

func checkOneSong() {
	rows := search()
	if rows == nil {
		return
	}
	if rows.Next() {
		var number int
		var artist, title, album string
		err := rows.Scan(&number, &artist, &title, &album)
		dirty, err := checkAllSites(title, artist)
		if err != nil {
			networkErrors.Printf("NETWORK ERROR: %s", err.Error())
		}
		var code string
		var log string
		if dirty {
			code = "E"
			log = "DIRTY"
		} else {
			code = "S"
			log = "CLEAN"
		}
		q := fmt.Sprintf("UPDATE CART SET SCHED_CODES='%s' WHERE NUMBER='%d'", code, number)
		err = query(q)
		if err != nil {
			networkErrors.Printf("SQL ERROR: %s", err.Error())
		}
		songClass.Printf("UPDATE: Title: %s Artist: %s (id %d) was marked %s\n", title, artist, number, log)
	}
}
