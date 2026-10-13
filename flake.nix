{
  description = "C 2048";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    let
      # Build the game from a given (possibly cross-compiling) pkgs set.
      # $CC and the host platform are provided by stdenv, so the same
      # recipe works natively and for every cross target.
      mk2048 = pkgs:
        pkgs.stdenv.mkDerivation {
          pname = "2048";
          version = "0.1.0";
          src = ./.;

          # Statically link the C runtime where the toolchain supports it
          # (musl for Linux) so the binary is portable.
          buildPhase = ''
            $CC -O2 -Wall -static -o 2048 main.c \
              || $CC -O2 -Wall -o 2048 main.c
          '';

          installPhase = ''
            mkdir -p $out/bin
            ext=${pkgs.stdenv.hostPlatform.extensions.executable}
            cp 2048$ext $out/bin/2048$ext
          '';
        };
    in
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        # Native build for the current host:
        #   - on x86_64/aarch64-darwin  -> a macOS binary
        #   - on x86_64/aarch64-linux   -> a Linux binary
        packages.default = mk2048 pkgs;

        # Cross builds (work from any host, incl. macOS):
        #   nix build .#linux-static -> static Linux binary (musl)
        packages.linux-static = mk2048 pkgs.pkgsCross.musl64;

        # `nix develop` gives you a shell with a C toolchain.
        devShells.default = pkgs.mkShell {
          packages = [ pkgs.gcc pkgs.gnumake ];
        };
      });
}
