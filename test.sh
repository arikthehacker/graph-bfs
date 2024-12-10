#!/usr/bin/env bash
# Build the graph program, run the sample session, and diff against expected output.
set -euo pipefail
cd "$(dirname "$0")"
CXX="${CXX:-g++}"
bin="$(mktemp -u).graph"
"$CXX" -std=c++17 -O2 -o "$bin" graph.cpp
"$bin" < sample-input.txt > "$bin.out" 2>&1 || true
rm -f "$bin"
if diff <(tr -d '\r' < "$bin.out") <(tr -d '\r' < expected-output.txt) >/dev/null; then
    rm -f "$bin.out"
    echo "PASS: output matches expected-output.txt"
    exit 0
else
    echo "FAIL: output differs from expected-output.txt"
    diff <(tr -d '\r' < "$bin.out") <(tr -d '\r' < expected-output.txt) | head -20
    rm -f "$bin.out"
    exit 1
fi
