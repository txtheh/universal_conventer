cmake_minimum_required(VERSION 3.19)
project(universal_conventer LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(universal_conventer
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    biginteger.h
    biginteger.cpp
    bigfraction.h
    bigfraction.cpp
    inputparser.h
    inputparser.cpp
    baseconverter.h
    baseconverter.cpp
    inputparser.h
    inputparser.h
    inputparser.h
    inputparser.h
    inputparser.cpp
    baseconverter.h
    baseconverter.cpp
)

target_link_libraries(universal_conventer
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS universal_conventer
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET universal_conventer
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
