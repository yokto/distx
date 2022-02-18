{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    buildInputs = [ pkgs.buildPackages.gmp ];
    nativeBuildInputs = [ pkgs.buildPackages.bison pkgs.buildPackages.flex pkgs.buildPackages.gnum4 ];
}
