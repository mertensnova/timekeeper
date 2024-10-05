cmake_minimum_required(VERSION 3.10)
project(WlrootsFocusedWindow C)

# Set C standard to C11 (you can adjust this as needed)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add WLR_USE_UNSTABLE macro
add_definitions(-DWLR_USE_UNSTABLE)

# Find pkg-config package
find_package(PkgConfig REQUIRED)

# Use pkg-config to find the necessary libraries
pkg_check_modules(WLROOTS REQUIRED wlroots)
pkg_check_modules(WAYLAND_SERVER REQUIRED wayland-server)

# Include the directories for wlroots and wayland-server
include_directories(${WLROOTS_INCLUDE_DIRS})
include_directories(${WAYLAND_SERVER_INCLUDE_DIRS})

# Add the executable
add_executable(wlroots_focused_window main.c)

# Link the necessary libraries to the executable
target_link_libraries(wlroots_focused_window ${WLROOTS_LIBRARIES} ${WAYLAND_SERVER_LIBRARIES})

# Ensure the RPATH is set for finding libraries at runtime
set_target_properties(wlroots_focused_window PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

