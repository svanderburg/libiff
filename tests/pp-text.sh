#!/bin/sh -e

test "x`../src/iffpp/iffpp pp-text.TEST | grep "Hello world"`" != "x"
