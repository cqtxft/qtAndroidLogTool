
SOURCES = main.cpp \
    mainwindow.cpp \
    customizeitem.cpp \
    controller.cpp \
    loginfo.cpp \
    datamodel.cpp \
    proxymodule.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/tutorials/widgets/nestedlayouts
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS nestedlayouts.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/tutorials/widgets/nestedlayouts
INSTALLS += target sources

symbian: include($$PWD/../../../symbianpkgrules.pri)
maemo5: include($$PWD/../../../maemo5pkgrules.pri)
contains(MEEGO_EDITION,harmattan): include($$PWD/../../../harmattanpkgrules.pri)

symbian: warning(This example might not fully work on Symbian platform)
maemo5: warning(This example might not fully work on Maemo platform)
contains(MEEGO_EDITION,harmattan): warning(This example might not fully work on Harmattan platform)
simulator: warning(This example might not fully work on Simulator platform)

HEADERS += \
    mainwindow.h \
    customizeitem.h \
    controller.h \
    loginfo.h \
    datamodel.h \
    proxymodule.h
