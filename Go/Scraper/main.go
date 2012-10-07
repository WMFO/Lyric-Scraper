package main

import (
	"database/sql"
)

func main() {
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
		err := query(q)
		if err != nil {
			// Log error
			// return
		}
		msg := fmt.Sprintf("UPDATE: %s by %s (id %d) was marked %s\n", title, artist, number, log)

	}
}
