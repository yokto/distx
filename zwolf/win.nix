with import <nixpkgs> {
  crossSystem = {
    config = "x86_64-w64-mingw32";
    libc = "msvcrt";
  };
};

mkShell {
  nativeBuildInputs = [ zlib ];
  buildInputs = [ zlib ];
}
