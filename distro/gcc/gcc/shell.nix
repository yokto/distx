with import <nixpkgs> {};
mkShell {
    buildInputs = [ gmp mpfr libmpc gcc ];
    nativeBuildInputs = [ bison flex gnum4 automake autoconf269 ];
    hardeningDisable = [ "format" ];
}
