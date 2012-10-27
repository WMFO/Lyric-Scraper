package main

import (
	"database/sql"
	"fmt"
)

const (
	USER = "Test"
	PASS = "Test"
)

var db sql.DB

func query(q string) error {
	_, err := db.Exec(q)
	return err
}

func search() *sql.Rows {
	query := "SELECT NUMBER,ARTIST,TITLE,ALBUM FROM CART WHERE SCHED_CODES IS NULL LIMIT 100"
	rows, err := db.Query(query)
	if err != nil {
		logMysqlError(err)
		return nil
	}
	return rows
}

func logMysqlError(err error) {
	// TODO
}

func connect(user, pass, dbname string) (*sql.DB, error) {
	return sql.Open("mysql", fmt.Sprintf("%s:%s@%s?charset=utf8", user, pass, dbname))
}
