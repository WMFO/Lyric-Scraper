package main

import (
	"log"
	"io"
)

var (
	networkErrors *log.Logger
	songClass    *log.Logger
	dirtySong     *log.Logger
	regexErrors   *log.Logger
)

func initLogging(net, song, dirty, regex io.Writer) {
	networkErrors = log.New(net, "", log.LstdFlags)
	songClass     = log.New(song, "", log.LstdFlags)
	dirtySong     = log.New(dirty, "", log.LstdFlags)
	regexErrors   = log.New(regex, "", log.LstdFlags)
}