package main

import (
	_ "code.google.com/p/go-mysql-driver/mysql"
	"database/sql"
	"fmt"
)

var db *sql.DB

func query(q string) error {
	_, err := db.Exec(q)
	return err
}

func search(n int) *sql.Rows {
	query := fmt.Sprintf("SELECT NUMBER,ARTIST,TITLE,ALBUM FROM CART WHERE SCHED_CODES IS NULL LIMIT %d", n)
	rows, err := db.Query(query)
	if err != nil {
		networkErrors.Printf("SQL ERROR: %s", err.Error())
		return nil
	}
	return rows
}

func connect(user, pass, dbname string) error {
	var err error
	db, err = sql.Open("mysql", fmt.Sprintf("%s:%s@/%s?charset=utf8", user, pass, dbname))
	return err
}
