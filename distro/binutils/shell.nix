{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    buildInputs = [ ];
    nativeBuildInputs = [ pkgs.buildPackages.bison pkgs.buildPackages.gpm pkgs.buildPackages.flex pkgs.buildPackages.gnum4 ];
}
