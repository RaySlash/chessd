{
  description = "Description for project";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    fenix = {
      url = "github:nix-community/fenix";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs = inputs @ {flake-parts, ...}:
    flake-parts.lib.mkFlake {inherit inputs;} {
      systems = [
        "aarch64-darwin"
        "aarch64-linux"
        "riscv64-linux"
        "x86_64-darwin"
        "x86_64-linux"
      ];

      perSystem = {
        system,
        pkgs,
        ...
      }: let
        rust-toolchain = pkgs.fenix.stable.withComponents [
          "cargo"
          "clippy"
          "rust-src"
          "rust-analyzer"
          "rustc"
          "rustfmt"
        ];
        deps = with pkgs; [
          pkg-config
          openssl
          raylib
          clang
          wayland
        ];
        libs = with pkgs;
          [
            pkg-config
            cmake
            coreutils
            bashInteractive
            stdenv.cc.libc_bin
            openlibm
            wayland-scanner
            egl-wayland
            eglexternalplatform
            libGL
            libclang
            libxkbcommon
            glfw3
          ]
          ++ (with pkgs.xorg; [
            libX11
            libX11.dev
            libXcursor
            libXext
            libXfixes
            libXi
            libXinerama
            libXrandr
            libXrender
          ]);
      in {
        _module.args.pkgs = import inputs.nixpkgs {
          inherit system;
          overlays = [inputs.fenix.overlays.default];
        };
        packages.default =
          (pkgs.makeRustPlatform {
            cargo = rust-toolchain;
            rustc = rust-toolchain;
          })
          .buildRustPackage {
            pname = "chessd";
            version = "0.0.1";
            src = ./.;
            cargoLock.lockFile = ./Cargo.lock;
            useFetchCargoVendor = true;

            LIBCLANG_PATH = "${pkgs.llvmPackages.libclang.lib}/lib";

            buildInputs = deps ++ libs;
            nativeBuildInputs = deps ++ libs;
          };
        devShells.default = pkgs.mkShell {
          shellHook = ''
            export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath libs}"
            export LIBCLANG_PATH="${pkgs.llvmPackages.libclang.lib}/lib"
          '';
          packages =
            [
              rust-toolchain
            ]
            ++ deps
            ++ libs;
        };
      };
    };
}
