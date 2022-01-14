#  This file is part of Hangman.
#  Copyright (C) 2008-2021 Thorsten Roth
#
#  Hangman is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Hangman is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Hangman.  If not, see <https://www.gnu.org/licenses/>.

TEMPLATE      = app

unix: !macx {
       TARGET = hangman
} else {
       TARGET = Hangman
}

win32:VERSION = 4.0.9.0
else:VERSION  = 4.0.9

QMAKE_TARGET_PRODUCT     = "Hangman"
QMAKE_TARGET_DESCRIPTION = "Hangman game framework written in C++ and Qt 5/6."
QMAKE_TARGET_COPYRIGHT   = "(C) 2008-2021 Thorsten Roth"

DEFINES      += APP_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\" \
                APP_VERSION=\"\\\"$$VERSION\\\"\" \
                APP_DESC=\"\\\"$$QMAKE_TARGET_DESCRIPTION\\\"\" \
                APP_COPY=\"\\\"$$QMAKE_TARGET_COPYRIGHT\\\"\"

MOC_DIR       = ./.moc
OBJECTS_DIR   = ./.objs
UI_DIR        = ./.ui
RCC_DIR       = ./.rcc

QT           += core gui widgets
CONFIG       += c++11

CONFIG(debug, debug|release) {
  CONFIG     += warn_on
  DEFINES    += QT_DEPRECATED_WARNINGS
  DEFINES    += QT_DISABLE_DEPRECATED_BEFORE=0x060000
}

SOURCES      += main.cpp\
                hangman.cpp \
                game.cpp

HEADERS      += hangman.h \
                game.h

FORMS        += hangman.ui

RESOURCES     = data/hangman_resources.qrc

TRANSLATIONS += lang/hangman_de.ts \
                lang/hangman_en.ts \
                lang/hangman_it.ts \
                lang/hangman_nl.ts

win32:RC_ICONS = data/win/icon.ico

macx {
  ICON               = data/mac/icon.icns
  QMAKE_INFO_PLIST   = data/mac/Info.plist
}

unix: !macx {
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
  isEmpty(BINDIR) {
    BINDIR = bin
  }

  target.path    = $$PREFIX/$$BINDIR/

  desktop.path   = $$PREFIX/share/applications
  desktop.files += data/unix/com.github.elth0r0.hangman.desktop

  pixmap.path    = $$PREFIX/share/pixmaps
  pixmap.files  += data/hangman.png \
                   data/unix/hangman.xpm

  meta.path      = $$PREFIX/share/metainfo
  meta.files    += data/unix/com.github.elth0r0.hangman.metainfo.xml

  INSTALLS      += target \
                   desktop \
                   pixmap \
                   meta
}
