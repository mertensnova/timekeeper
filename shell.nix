let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
    packages = [
      pkgs.gtest
      pkgs.pkg-config
      pkgs.cmake
      pkgs.meson
      pkgs.ninja
      pkgs.libinput
      pkgs.wayland-scanner
      pkgs.waylandpp
    ];
  }
