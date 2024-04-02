#!/bin/sh -e

../src/iffjoin/iffjoin -o join.IFF join.HELO join.HELO
../src/iffcheck/iffcheck --level 0 join.IFF
