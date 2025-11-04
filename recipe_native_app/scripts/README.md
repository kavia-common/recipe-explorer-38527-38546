# CI Scripts for recipe_native_app

This folder contains minimal shell scripts used by the CI "common container setup" phase.

- setup.sh: Runs early in the setup. It performs no heavy work but ensures a valid shell script exists to avoid EOF parsing errors in certain bash invocations.
- post_process.sh: Writes a SUCCESS marker to post_process_status.lock to indicate the container has completed its post-processing step.

Notes:
- These scripts use UNIX line endings and bash with `set -euo pipefail` to catch errors.
- If future setup steps are required (e.g., downloading assets), add them to setup.sh.
