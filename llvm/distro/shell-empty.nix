with import <nixpkgs> {};
gccStdenv.mkDerivation {
    name = "myclang";
    buildInputs = [ ];
    nativeBuildInputs = [ ninja cmakeCurses bison flex gnum4 automake autoconf269 ];
    hardeningDisable = [ "format" ];
}
