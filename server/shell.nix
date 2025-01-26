{ pkgs ? import <nixpkgs> {} }:

let
  linkToNixPaths = pkgs.runCommand "link-paths" {} ''
    mkdir -p $out/include
    mkdir -p $out/lib

    ln -s ${pkgs.libmysqlconnectorcpp}/include/* $out/include/
    ln -s ${pkgs.libmysqlconnectorcpp}/lib/* $out/lib/
  '';

in pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    gcc
    gnumake
    libmysqlconnectorcpp
    gtest
  ];

  shellHook = ''
    export NIX_CFLAGS_COMPILE="-isystem ${linkToNixPaths}/include $NIX_CFLAGS_COMPILE"
    export NIX_LDFLAGS="-L${linkToNixPaths}/lib $NIX_LDFLAGS"
    export CMAKE_PREFIX_PATH=${linkToNixPaths}:$CMAKE_PREFIX_PATH
  '';
}
