QT += widgets
CONFIG += c++11
SOURCES += \
    companypage.cpp \
    main.cpp \
    mainmenupage.cpp \
    mainwindow.cpp \
    projectpage.cpp \
    taskpage.cpp
HEADERS += \
    companypage.h \
    mainmenupage.h \
    mainwindow.h \
    projectpage.h \
    taskpage.h
FORMS += \
    companypage.ui \
    mainmenupage.ui \
    mainwindow.ui \
    projectpage.ui \
    taskpage.ui
RESOURCES += \
    resources.qrc

#add mac and windows icons because why not?
win32:RC_ICONS = images/favicon.ico
macx:ICON = images/icon.icns
