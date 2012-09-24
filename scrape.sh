#!/bin/bash
./Scraper.out "$1" "$2" | egrep -o -f filter.txt
