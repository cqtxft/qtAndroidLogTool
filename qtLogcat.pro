/*
 * Copyright (C) 2012  cqtxft@gmail.com
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
