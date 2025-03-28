#!/bin/bash

CURRENT_DIR=$(dirname "$(realpath "$0")")
PYCGRAPH_TUTORIAL_DIR="$CURRENT_DIR/tutorial/"

# shellcheck disable=SC2164
cd "$PYCGRAPH_TUTORIAL_DIR"

for file in *.py; do
    [[ "$(basename "$file")" == "__init__.py" ]] && continue
    echo "**** Running $file ****"
    python3 "$file"
    echo " "
done

echo -e "\033[34m congratulations, automatic run PyCGraph tutorials finish... \033[0m"
