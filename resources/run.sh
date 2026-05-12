#!/bin/bash
DIR=$(cd "$(dirname "$0")/.." && pwd)
BUILD="$DIR/build"

cd "$BUILD"
if [ -e wiikey ]; then
    ./wiikey
else
    echo "No wiikey file"
    exit 1
fi
