Lyric-Scraper
=============

WMFO - Tufts Freeform Radio  
ops@wmfo.org  
For copyrights and licensing, see COPYING.  

Determines the profanity, if any, in a song by curling lyrics from multiple
websites.  

The code currently scrapes only two sites, but is otherwise in working order.  

Usage
-----
At the moment, the Makefile is out of date. Please do not use it.  

More usage information will be added later. Sorry.  

Changelog
---------
###08/15/12
Initial incomplete C++ version. - Nate Tarrh

###09/11/12
Overhaul architecture to put site-specific code in well-defined functions. Still doesn't work. - Max Goldstein

###09/23/12
Portability bug fixes, add another site. - Connor Blanck

###09/23/12
Move string parsing functions around. Call lyrics function in loop. - Max Goldstein

###09/23/12
Added more buffering to LyricsCom so it doesn't grab as much HTML. Fixed scrape.sh so it allows for songs with spaces. - Connor Blanck

###02/16/13
Merge in Go version from fork. - Connor Blanck
Delete old, non-working C++ implementation. - Max Goldstein
