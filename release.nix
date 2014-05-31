{ nixpkgs ? <nixpkgs>
, systems ? [ "i686-linux" "x86_64-linux" ]
, buildForAmiga ? false
, buildForWindows ? false
, amigaosenvPath ? <amigaosenv>
, libiff ? { outPath = ./.; rev = 1234; }
, officialRelease ? false
}:

let
  pkgs = import nixpkgs {};
  
  version = builtins.readFile ./version;
  
  jobs = rec {
    tarball =
      with pkgs;

      releaseTools.sourceTarball {
        name = "libiff-tarball";
        src = libiff;
        inherit version officialRelease;

        buildInputs = [];
      };
      
    build =
      (pkgs.lib.genAttrs systems (system:
        with import nixpkgs { inherit system; };
        
        releaseTools.nixBuild {
          name = "libiff";
          inherit version;
          src = tarball;
          CFLAGS = "-ansi -pedantic -Wall";
        }
      )) //
      (pkgs.lib.optionalAttrs (buildForWindows) { i686-windows =
         pkgs.dotnetenv.buildSolution {
           name = "libiff";
           src = ./.;
           baseDir = "src";
           slnFile = "libiff.sln";
           preBuild = ''
             sed "s/@IFF_BIG_ENDIAN@/0/" libiff/ifftypes.h.in > libiff/ifftypes.h
           '';
           postInstall = ''
             mkdir -p $out/include/libiff
             cp -v libiff/*.h $out/include/libiff
           '';
         };
        }) //
      (pkgs.lib.optionalAttrs (buildForAmiga)
        (let
          amigaosenv = import amigaosenvPath {
            inherit (pkgs) stdenv uae procps;
            lndir = pkgs.xorg.lndir;
          };
        in
        {
          m68k-amigaos.lib = amigaosenv.mkDerivation {
            name = "libiff-${version}";
            src = "${tarball}/tarballs/libiff-${version}pre1234.tar.gz";
      
            buildCommand = ''
              tar xfvz $src
              cd libiff-${version}pre1234
              CFLAGS='-noixemul -O3' ./configure --prefix=/OUT --disable-shared
              cd src/libiff
              make
              make install
            '';
          };
        
          m68k-amigaos.tools = amigaosenv.mkDerivation {
            name = "libiff-${version}";
            src = "${tarball}/tarballs/libiff-${version}pre1234.tar.gz";
      
            buildCommand = ''
              tar xfvz $src
                cd libiff-${version}pre1234
               ./configure --prefix=/OUT --disable-shared
               make
               make check
               make install
            '';
          };
        }));
  };
in
jobs
