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
	connect("user", "pass", "mysqldb")
	checkOneSong()
}

func checkOneSong() {
	rows := search()
	if rows.Next() {
		var number int
		var artist, title, album string
		err := rows.Scan(&number, &artist, &title, &album)
		dirty, err := checkAll(title, artist)
		if err != nil {
			// TODO
			// Log error
			// return
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
			// TODO
			// Log error
			// return
		}
		msg := fmt.Sprintf("UPDATE: %s by %s (id %d) was marked %s\n", title, artist, number, log)

	}
}
