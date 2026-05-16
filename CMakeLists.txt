cmake_minimum_required(VERSION 3.21)

project(flarecore LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/htmldialogs
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt;/opt/homebrew/opt/qttools")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets UiTools)

qt_add_executable(flarecore
    codeeditor.cpp
    codeeditor.h
    completer.cpp
    completer.h
    cssdocument.cpp
    cssdocument.h
    document.cpp
    document.h
    finddialog.cpp
    finddialog.h
    FindDiag.ui
    flare.cpp
    flare.h
    flare.ui
    flexdocument.cpp
    flexdocument.h
    foundry.cpp
    foundry.h
    globals.h
    htmldocument.cpp
    htmldocument.h
    insertbuttongroup.cpp
    insertbuttongroup.h
    insertbuttongroup.ui
    insertregistrationform.cpp
    insertregistrationform.h
    resources/htmldialogs/insertregistrationform.ui
    insertselectmenudialog.cpp
    insertselectmenudialog.h
    jsdocument.cpp
    jsdocument.h
    main.cpp
    manageprojectsdialog.cpp
    manageprojectsdialog.h
    manageprojectsdialog.ui
    newfiledialog.cpp
    newfiledialog.h
    newfiledialog.ui
    phpdocument.cpp
    phpdocument.h
    project.cpp
    project.h
    projectdialog.cpp
    projectdialog.h
    projectdialog.ui
    resources.qrc
    settings.cpp
    settings.h
    switchproject.cpp
    switchproject.h
    switchproject.ui
    syntaxhighlighter.cpp
    syntaxhighlighter.h
    textdocument.cpp
    textdocument.h
)

target_link_libraries(flarecore PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::UiTools
)

target_include_directories(flarecore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(flarecore PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
