QT += core gui widgets multimedia \
      charts



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Charactersettingspage.cpp \
    Flowlayout.cpp \
    PhotoFrameManager.cpp \
    albumpage.cpp \
    backgroundsettingspage.cpp \
    dateselectdialog.cpp \
    gamepage.cpp \
    gamesettingspage.cpp \
    gameview.cpp \
    main.cpp \
    mainwindow.cpp \
    musicsettingspage.cpp \
    photoframewidget.cpp \
    settingspage.cpp \
    yearlyreportdialog.cpp

HEADERS += \
    Charactersettingspage.h \
    ClickableLabel.h \
    Flowlayout.h \
    PhotoFrameData.h \
    PhotoFrameItem.h \
    PhotoFrameManager.h \
    albumpage.h \
    backgroundsettingspage.h \
    dateselectdialog.h \
    gamepage.h \
    gamesettingspage.h \
    gameview.h \
    mainwindow.h \
    musicsettingspage.h \
    photoframewidget.h \
    settingspage.h \
    yearlyreportdialog.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    backgrounds.qrc \
    characters.qrc

DISTFILES +=

FORMS += \
    yearlyreportdialog.ui
