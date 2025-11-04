#!/usr/bin/env bash
# Purpose: Minimal setup script for CI common container setup in recipe_native_app.
# Ensures a valid shell script exists to prevent "unexpected end of file" errors.

set -euo pipefail

echo "[recipe_native_app] Running setup.sh"
# No special setup required for this native app. Placeholder to satisfy CI harness.
# Keep as two or more lines to avoid edge-case EOF parsing issues in some runtimes.

exit 0
