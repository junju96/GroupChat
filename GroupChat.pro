QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    avatarwidget/avatarwidget.cpp \
    database/chatdatabase.cpp \
    groupchatwindow/groupchatwindow.cpp \
    groupmemberdialog.cpp \
    main.cpp \
    messagebubble/messagebubble.cpp \
    messageitem/messageitem.cpp

HEADERS += \
    groupchatconfig.h \
    avatarwidget/avatarwidget.h \
    database/chatdatabase.h \
    groupchatwindow/groupchatwindow.h \
    groupmemberdialog.h \
    messagebubble/messagebubble.h \
    messageitem/messageitem.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
