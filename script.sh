#!/bin/bash

OUTPUT="words.txt"
WORD="example"

yes "$WORD" | head -n 10000000 > "$OUTPUT"

echo "File generated: $OUTPUT"
