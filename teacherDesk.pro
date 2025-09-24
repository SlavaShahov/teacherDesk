QT       += core gui sql widgets

CONFIG += c++17

# Set version and language
TARGET = teacherDesk
TEMPLATE = app
VERSION = 0.1

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    workplace.cpp \
    coachpage.cpp

HEADERS += \
    mainwindow.h \
    workplace.h \
    coachpage.h

FORMS += \
    mainwindow.ui \
    workplace.ui \
    coachpage.ui

RESOURCES += \
    resources.qrc

# For iOS/macOS, you can set the bundle identifier here (for Qt 5/6):
# macx {
#    QMAKE_BUNDLE_ID = com.example.teacherDesk
#    QMAKE_MACOSX_BUNDLE = 1
# }

# If you need to specify the installation directories, you can use these:
# DESTDIR = path/to/your/destination
# INSTALLS += target

# Uncomment for Android setup (Qt 6 version):
# android {
#     ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
# }
