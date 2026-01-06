cmake_minimum_required(VERSION 3.16)
project(Diary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ----------------------
# Find required Qt modules
# ----------------------
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# ----------------------
# Find OpenSSL
# ----------------------
find_package(OpenSSL REQUIRED)

# ----------------------
# Define executable
# ----------------------
add_executable(Diary
    main.cpp
    borderpulseoverlay.cpp
    CryptoUtils.cpp
    journal_system.cpp
    nameprompt.cpp

    borderpulseoverlay.h
    CryptoUtils.h
    Entry.h
    journal_system.h
    Journal.h
    nameprompt.h

    journal_system.ui
    nameprompt.ui
)

# ----------------------
# Link against Qt libraries + OpenSSL
# ----------------------
target_link_libraries(Diary PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets OpenSSL::Crypto OpenSSL::SSL)

# ----------------------
# macOS-specific settings
# ----------------------
if(APPLE)
    # Build as a macOS bundle
    set_target_properties(Diary PROPERTIES
        MACOSX_BUNDLE TRUE
        INSTALL_RPATH "@executable_path/../Frameworks"
    )

    # Set a fixed bundle identifier so AppDataLocation is stable
    set_target_properties(Diary PROPERTIES
        MACOSX_BUNDLE_BUNDLE_IDENTIFIER "com.matthewholguin.Diary"
    )
endif()
