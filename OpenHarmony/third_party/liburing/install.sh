#!/bin/bash
# SPDX-License-Identifier: MIT

SRC_DIR="$1"
CODE_DIR="$2"

set -e
if [ "$SRC_DIR" == "" ] || [ "$CODE_DIR" == "" ]; then
    exit 1
fi

mkdir -p $CODE_DIR
cp -r $SRC_DIR $CODE_DIR

CURRENT_DIR=$(pwd)
cd $CODE_DIR/liburing
sh ./configure
cd "$CURRENT_DIR"
