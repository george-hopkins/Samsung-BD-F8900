NSIS := c:\/Program\ Files/NSIS/makensis.exe

RM := -/bin/rm -f
CP := -/bin/cp -f
MV := -/bin/mv
MKDIR := -/bin/mkdir -p
ECHO := echo
FIND := find
AWK := gawk
TAR := tar
GREP := grep
PERL := /usr/bin/perl
RMDIR := -rmdir

###############################################################################
#
#                (c) Cambridge Silicon Radio Limited 2009
#
#                All rights reserved and confidential information of CSR
#
###############################################################################
path_subst = $(subst \,/,$1)