#!/bin/bash
#mysql_result=`mysql -u <user> <schema> <<< "SELECT NUMBER,ARTIST,TITLE,ALBUM FROM CART WHERE SCHED_CODES IS NULL LIMIT 100"`
#mysql_result=`echo "$mysql_result" | sed 's/\t/\n/g'` #replace tabs with newlines
./Scraper.out "$1" "$2" | egrep -o -f filter.txt
