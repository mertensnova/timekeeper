let
  pkgs = import <nixpkgs> {};
in
  pkgs.mkShell {
    packages = [

      pkgs.wlroots_0_17
      pkgs.wayland
      pkgs.gtest
      pkgs.pkg-config
      pkgs.cmake
      pkgs.meson
      pkgs.ninja
      pkgs.libinput
      pkgs.wayland-scanner
    ];
  }
