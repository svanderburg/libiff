#!/bin/sh -e

../src/iffjoin/iffjoin -o join.IFF join.HELO join.BYE
./validiff join.IFF
