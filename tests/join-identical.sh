#!/bin/sh -e

../src/iffjoin/iffjoin -o join.IFF join.HELO join.HELO
./validiff join.IFF
