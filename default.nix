# import <nixpkgs>{} {
#{stdenv, gcc, coreutils}: {
# with (import <nixpkgs>{}): {
# import <nixpkgs>{} : {
let
    pkgs = import <nixpkgs> {};
    stdenv = pkgs.stdenv;
in with pkgs; {
  hgen = stdenv.mkDerivation {
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
  };
}
