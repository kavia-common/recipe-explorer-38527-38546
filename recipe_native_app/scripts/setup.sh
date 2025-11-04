#!/usr/bin/env bash
# Purpose: Setup script for CI common container setup in recipe_native_app.
# Installs required system dependencies so CMake can find Qt6 and XKB.

set -euo pipefail

echo "[recipe_native_app] Running setup.sh"

# Non-interactive apt operations
export DEBIAN_FRONTEND=noninteractive

# Update package lists quietly and install required packages
# - qt6-base-dev: Qt6 Core/Gui/Widgets headers and tools
# - libxkbcommon-dev and libxkbcommon-x11-dev: XKB libraries required by Qt Gui (>= 0.5.0)
# - libxcb-xkb-dev: XCB XKB headers used by xkbcommon-x11
if command -v apt-get >/dev/null 2>&1; then
  sudo apt-get update -y
  sudo apt-get install -y \
    qt6-base-dev \
    libxkbcommon-dev \
    libxkbcommon-x11-dev \
    libxcb-xkb-dev
else
  echo "[recipe_native_app] Note: apt-get not available; ensure Qt6 and XKB dev packages are installed in the base image."
fi

exit 0
