# $Id: Makefile 5891 2009-07-20 12:53:09Z bogdan_iancu $
#
#  db_berkeley Makefile
# 

include ../../Makefile.defs

auto_gen=
NAME=bdb_recover

include ../../Makefile.sources

# if you want to tune or reset flags
#DEFS:=-DEXTRA_DEBUG  -I$(LOCALBASE)/BerkeleyDB.4.6/include
DEFS+=-I$(LOCALBASE)/include -I$(LOCALBASE)/BerkeleyDB.4.6/include \
        -I$(SYSBASE)/include
        
LIBS=-L$(LOCALBASE)/lib -L$(SYSBASE)/lib -L$(LOCALBASE)/BerkeleyDB.4.6/lib -ldb

include ../../Makefile.rules

modules:

