INCLUDEPATH += $$PWD/
___HAS_GIT = 0
system('git rev-parse --is-inside-work-tree') {
    DEFINES += SQPROJECT_HAS_GIT
    ___HAS_GIT = 1
}
equals(___HAS_GIT, 1) {
    GIT_COMMIT = '\\"$$system(git rev-parse --verify main)\\"'
    GIT_TAG = '\\"$$system(git describe --tags --exact-match)\\"'
    !isEmpty(GIT_COMMIT) {
        DEFINES += SQPROJECT_GIT_COMMIT=\"$${GIT_COMMIT}\"
    }
    !isEmpty(GIT_TAG) {
        DEFINES += SQPROJECT_GIT_TAG=\"$${GIT_TAG}\"
    }
}


SOURCES +=  $$PWD/sqapplication.cpp

HEADERS += $$PWD/sqapplication.h

