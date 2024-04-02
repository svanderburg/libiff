#!/bin/sh -e

../src/iffjoin/iffjoin -o join.IFF join.HELO join.BYE
../src/iffcheck/iffcheck --level 0 join.IFF
