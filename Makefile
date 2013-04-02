#Makefile for Lyric-Scraper
#Suggested usage: git pull
#                 sudo make


INSTALLDIR=/opt/wmfo/lyric-scraper
OWNER = root
MOD = 755
FILE = Scraper

.PHONY: all clean install uninstall

all:
    go get github.com/go-sql-driver/mysql
    go fmt *.go
    go build -o $FILE
    touch {networkErrors,songClassification,dirtySongs,regex}.log
