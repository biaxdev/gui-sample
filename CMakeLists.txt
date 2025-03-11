cmake_minimum_required(VERSION 3.5.1)

project(sensor-dashboard VERSION 1.0.0 LANGUAGES CXX)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(CAIROMM REQUIRED cairomm-1.0)

find_package(nlohmann_json REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    canvas.cpp
)

include_directories(
    ${GTKMM_INCLUDE_DIRS}
    ${CAIROMM_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    nlohmann_json::nlohmann_json
    ${GTKMM_LIBRARIES}
    ${CAIROMM_LIBRARIES}
)

# Add other necessary link directories
link_directories(${GTKMM_LIBRARY_DIRS} ${CAIROMM_LIBRARY_DIRS})

# Add compiler flags
add_definitions(${GTKMM_CFLAGS_OTHER} ${CAIROMM_CFLAGS_OTHER})