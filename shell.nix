{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    gcc
    gnumake

    python3
    python3Packages.pyqt5
    python312Packages.pybind11
    qt5.qtbase
    qt5.wrapQtAppsHook
    xorg.libX11
    xorg.libXrender
    xorg.libxcb
    libGL
    libxkbcommon
  ];

  shellHook = ''
    export QT_QPA_PLATFORM=xcb
    export QT_PLUGIN_PATH="${pkgs.qt5.qtbase.bin}/${pkgs.qt5.qtbase.qtPluginPrefix}"
    export QT_QPA_PLATFORM_PLUGIN_PATH="${pkgs.qt5.qtbase.bin}/${pkgs.qt5.qtbase.qtPluginPrefix}/platforms"
    export LD_LIBRARY_PATH="${pkgs.qt5.qtbase}/lib:${pkgs.xorg.libX11}/lib:${pkgs.xorg.libxcb}/lib:${pkgs.libGL}/lib:${pkgs.libxkbcommon}/lib''${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH"
  '';
}
