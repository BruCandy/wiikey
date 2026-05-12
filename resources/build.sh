#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/build"

rm -rf "$BUILD"
mkdir -p "$BUILD"
cmake -S "$DIR" -B "$BUILD"
cmake --build "$BUILD"
