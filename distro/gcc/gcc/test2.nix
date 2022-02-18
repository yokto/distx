with import <nixpkgs> {};
derivation {
  name = "simple";
  builder = "${bash}/bin/bash";
  nativeBuildInputs = [ coreutils ];
  args = [ "./test.sh" ];
  src = ./test.nix;
  system = builtins.currentSystem;
}
