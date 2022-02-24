with import <nixpkgs> {};
mkShell {
    buildInputs = [ gmp mpfr libmpc gcc ];
    nativeBuildInputs = [ bison flex gnum4 ];
}
