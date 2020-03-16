#{stdenv, gcc, coreutils}:
with (import <nixpkgs>{});

stdenv.mkDerivation {
  pname = "hgen";
  version = "1.0";
  src = ./.;
  buildInputs = [
    gcc
    coreutils
  ];
  installPhase =''
    mkdir $out
    mkdir $out/bin
    make
    cp hgen $out/bin
  '';
}
