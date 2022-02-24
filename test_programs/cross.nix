with import <nixpkgs> {
  crossSystem = {
    config = "aarch64-unknown-linux-gnu";
  };
};

mkShell {
  # nativeBuildInputs = [ gcc ];
  buildInputs = [ zlib ]; # your dependencies here
}
