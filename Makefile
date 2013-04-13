#Makefile for Lyric-Scraper
#Suggested usage: git pull
#                 sudo make all


INSTALLDIR=/opt/wmfo/lyric-scraper
OWNER = root
MOD = 755
FILE = Scraper

.PHONY: all clean install uninstall

all: mysql
	@go fmt *.go
	@go build -o $(FILE)
	@touch networkErrors.log
	@touch songClassification.log
	@touch dirtySongs.log
	@touch regex.log
	@sudo chmod 777 *.log

mysql: 
	@go get github.com/go-sql-driver/mysql
	@go build -a -o $(FILE)
