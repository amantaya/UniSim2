#-------------------------------------------------
#
# DLL
#
#-------------------------------------------------

BOXES_PLUGIN_NAME = eq

include("$$_PRO_FILE_PWD_/../../boxes_plugin.pri")

HEADERS +=  \
    exponential.h \
    gompertz.h \
    transition.h
	
SOURCES +=  \
    exponential.cpp \
    gompertz.cpp \
    transition.cpp


