with import <nixpkgs> {};
clangStdenv.mkDerivation {
    name = "myclang";
    buildInputs = [ gmp mpfr libmpc clang libllvm];
    nativeBuildInputs = [ ninja cmakeCurses bison flex gnum4 automake autoconf269 ];
    hardeningDisable = [ "format" ];
}
