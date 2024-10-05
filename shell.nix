let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
     buildInputs = [ pkgs.wayland pkgs.wayland-protocols ];
    packages = [
      pkgs.libxkbcommon
      pkgs.pixman
      pkgs.wlroots_0_17
      pkgs.gtest
      pkgs.pkg-config
      pkgs.cmake
      pkgs.meson
      pkgs.ninja
      pkgs.libinput
      pkgs.wayland-scanner
    ];
  }
