#{stdenv, gcc, coreutils}:
with (import <nixpkgs>{});

stdenv.mkDerivation {
  pname = "HGen";
  version = "1.0";
  src = ./.;
  buildInputs = [
    gcc
    coreutils
  ];
  installPhase =''
    mkdir $out
    make
    cp hgen $out
  '';
}
