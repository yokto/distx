with import <nixpkgs> {};
  pkgs.mkShell {
    buildInputs = [ gmp libtool ];
    nativeBuildInputs = [ bison flex gnum4 automake autoconf269 libtool ];
}
