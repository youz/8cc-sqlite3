SRC := src/8cc.c.eir.sqlite3 src/elc.c.eir.sqlite3
TARGETS := supported_targets.txt
MKRB := mksql.rb
SQL := elvm.sql
DB := elvm.db3

DIST := elvm-sqlite3.zip
DISTFILES := elvm.db3 README.md LICENSE samples

all: $(DB)

$(SQL): $(SRC) $(MKRB) $(TARGETS)
	ruby $(MKRB) $@ $(SRC)

$(DB): $(SQL)
	if [ -f $(DB) ]; then rm $(DB); fi;
	sqlite3 $(DB) < $(SQL)

dist: $(DB) $(DISTFILES)
	7za a $(DIST) $(DISTFILES)

clean:
	rm $(DB) $(SQL)
