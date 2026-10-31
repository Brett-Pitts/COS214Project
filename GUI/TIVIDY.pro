QT += widgets
CONFIG += c++11
CONFIG += console
INCLUDEPATH += $$PWD/../src

# Redirect the final executable/library output
DESTDIR = $$PWD/..

# Redirect intermediate build artifacts
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
RCC_DIR = $$PWD/build/rcc
UI_DIR = $$PWD/build/ui
SOURCES += \
    companypage.cpp \
    loadsavepage.cpp \
    main.cpp \
    mainmenupage.cpp \
    mainwindow.cpp \
    projectpage.cpp \
    taskpage.cpp
SOURCES += \
    $$PWD/../src/Aggregate.cpp \
    $$PWD/../src/ApplicationInterface.cpp \
    $$PWD/../src/Cancelled.cpp \
    $$PWD/../src/CompanyNode.cpp \
    $$PWD/../src/Completed.cpp \
    $$PWD/../src/ConcreteEmployeeBuilder.cpp \
    $$PWD/../src/ConcreteWorkMediator.cpp \
    $$PWD/../src/Department.cpp \
    $$PWD/../src/Employee.cpp \
    $$PWD/../src/EmployeeCompetency.cpp \
    $$PWD/../src/EmployeeComponent.cpp \
    $$PWD/../src/EmployeeDetail.cpp \
    $$PWD/../src/EmployeeDirector.cpp \
    $$PWD/../src/EmployeeProfession.cpp \
    $$PWD/../src/Escalated.cpp \
    $$PWD/../src/Failed.cpp \
    $$PWD/../src/FileSystem.cpp \
    $$PWD/../src/Iterator.cpp \
    $$PWD/../src/Manager.cpp \
    $$PWD/../src/Memento.cpp \
    $$PWD/../src/Ongoing.cpp \
    $$PWD/../src/Project.cpp \
    $$PWD/../src/Ready.cpp \
    $$PWD/../src/Team.cpp \
    $$PWD/../src/TeamIterator.cpp \
    $$PWD/../src/WaitingForPrerequisites.cpp \
    $$PWD/../src/WorkIterator.cpp \
    $$PWD/../src/WorkNode.cpp \
    $$PWD/../src/WorkState.cpp \
    $$PWD/../src/WorkUnit.cpp
HEADERS += \
    companypage.h \
    loadsavepage.h \
    mainmenupage.h \
    mainwindow.h \
    projectpage.h \
    taskpage.h
HEADERS += \
    $$PWD/../src/Aggregate.h \
    $$PWD/../src/ApplicationInterface.h \
    $$PWD/../src/Cancelled.h \
    $$PWD/../src/CompanyNode.h \
    $$PWD/../src/Completed.h \
    $$PWD/../src/ConcreteEmployeeBuilder.h \
    $$PWD/../src/ConcreteWorkMediator.h \
    $$PWD/../src/Department.h \
    $$PWD/../src/Employee.h \
    $$PWD/../src/EmployeeBuilder.h \
    $$PWD/../src/EmployeeCompetency.h \
    $$PWD/../src/EmployeeComponent.h \
    $$PWD/../src/EmployeeDetail.h \
    $$PWD/../src/EmployeeDirector.h \
    $$PWD/../src/EmployeeProfession.h \
    $$PWD/../src/Escalated.h \
    $$PWD/../src/Failed.h \
    $$PWD/../src/FileSystem.h \
    $$PWD/../src/Iterator.h \
    $$PWD/../src/Manager.h \
    $$PWD/../src/Mediator.h \
    $$PWD/../src/Memento.h \
    $$PWD/../src/MementoType.h \
    $$PWD/../src/Ongoing.h \
    $$PWD/../src/Project.h \
    $$PWD/../src/Ready.h \
    $$PWD/../src/Team.h \
    $$PWD/../src/TeamIterator.h \
    $$PWD/../src/WaitingForPrerequisites.h \
    $$PWD/../src/WorkIterator.h \
    $$PWD/../src/WorkMediator.h \
    $$PWD/../src/WorkNode.h \
    $$PWD/../src/WorkState.h \
    $$PWD/../src/WorkUnit.h
FORMS += \
    companypage.ui \
    loadsavepage.ui \
    mainmenupage.ui \
    mainwindow.ui \
    projectpage.ui \
    taskpage.ui
RESOURCES += \
    resources.qrc
win32:RC_ICONS = images/favicon.ico
