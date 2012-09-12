#Makefile for Lyric-Scraper
#Suggested usage: git pull
#                 sudo make install
#
#Compile without installing: make all

INSTALLDIR=/opt/wmfo/lyric-scraper
OWNER = root
MOD = 755
FILE = Scraper
OTHERFILES = Lyrics.cpp
OTHERPREREQS = Lyrics.h filter.txt

.PHONY: all clean install uninstall

all: $(FILE).out

$(FILE).out: $(FILE).cpp $(OTHERFILES) $(OTHERPREREQS)
	@g++ -o $@ $< $(OTHERFILES) -Wall -Wextra -Werror -lcurl

clean:
	@$(RM) *.o $(FILE).out

install: $(INSTALLDIR)/$(FILE).rlm

$(INSTALLDIR)/$(FILE).rlm: $(FILE).rlm
	@mkdir -p $(INSTALLDIR)
	@if [ -f $@ ] ; \
	then \
		cp $@ $@.bak ; \
	fi;
	@cp $< $@
	@chown $(OWNER) $@
	@chmod $(MOD) $@

uninstall:
	@$(RM) $(INSTALLDIR)/$(FILE).rlm
	@if [ -f $(INSTALLDIR)/$(FILE).rlm.bak ] ; \
	then \
		cp $(INSTALLDIR)/$(FILE).rlm.bak $(INSTALLDIR)/$(FILE).rlm ; \
		$(RM) $(INSTALLDIR)/$(FILE).rlm.bak ; \
	fi;
