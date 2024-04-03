#!/bin/sh -e

test "x`./testpp extension.TEST | grep 4096`" != "x"
