RM := - del /f
CP := - copy
MV := - move
MKDIR := - mkdir
ECHO := echo
FIND := dir /s /b 
path_subst = $(subst /,\,$1)

USE_DEPENDENCY_FILES := 0

