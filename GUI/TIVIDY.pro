QT += widgets
CONFIG += c++11
SOURCES += \
    addemployeedialog.cpp \
    companypage.cpp \
    main.cpp \
    mainmenupage.cpp \
    mainwindow.cpp \
    namedialog.cpp \
    projectpage.cpp
HEADERS += \
    addemployeedialog.h \
    companypage.h \
    mainmenupage.h \
    mainwindow.h \
    namedialog.h \
    projectpage.h
FORMS += \
    addemployeedialog.ui \
    companypage.ui \
    mainmenupage.ui \
    mainwindow.ui \
    namedialog.ui \
    projectpage.ui
RESOURCES += \
    resources.qrc

#add mac and windows icons because why not?
win32:RC_ICONS = images/favicon.ico
macx:ICON = images/icon.icns
