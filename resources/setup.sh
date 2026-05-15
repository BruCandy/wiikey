#!/bin/bash
DIR=$(cd "$(dirname "$0")" && pwd)
APP_PATH="$DIR/../build/wiikey"
DESKTOP_DIR="$HOME/.local/share/applications"
ICON_DIR="$HOME/.local/share/icons/hicolor"
ICON_APP_DIR="$ICON_DIR/48x48/apps"

mkdir -p "$DESKTOP_DIR"
cp "$DIR/../data/wiikey.desktop" "$DESKTOP_DIR/wiikey.desktop"
sed -i "s|Exec=wiikey|Exec=$APP_PATH|" "$DESKTOP_DIR/wiikey.desktop"

mkdir -p "$ICON_APP_DIR"
cp "$DIR/../data/icons/wiikey_icon_48.png" "$ICON_APP_DIR/wiikey_icon_48.png"

mkdir -p "$ICON_DIR"
cp "$DIR/../data/theme/index.theme" "$ICON_DIR/index.theme"

gtk-update-icon-cache "$ICON_DIR"
