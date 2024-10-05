find_package(Wayland REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols)

# Generate the XDG shell protocol
wayland_scanner(client-header
    "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml"
    "${CMAKE_BINARY_DIR}/xdg-shell-protocol.h"
)

wayland_scanner(private-code
    "${WAYLAND_PROTOCOLS_DIR}/stable/xdg-shell/xdg-shell.xml"
    "${CMAKE_BINARY_DIR}/xdg-shell-protocol.c"
)

include_directories(${CMAKE_BINARY_DIR})

add_executable(my_program
    main.c
    ${CMAKE_BINARY_DIR}/xdg-shell-protocol.c
)

