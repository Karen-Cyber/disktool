AUX_SOURCE_DIRECTORY(. SRC_LIST)

IF (DEBUG)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF()

IF(SRC_LIST)
    ADD_LIBRARY(mysrc STATIC ${SRC_LIST})
ENDIF()