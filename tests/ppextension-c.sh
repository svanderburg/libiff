#!/bin/sh -e

test "x`./ppextension extension.TEST | grep 4096`" != "x"
