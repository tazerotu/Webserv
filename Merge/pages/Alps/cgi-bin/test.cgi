#!/bin/bash

# Read QUERY_STRING
QUERY="$QUERY_STRING"

# Decode + replace
DECODED=$(printf '%b' "${QUERY//%/\\x}")

# Print to terminal / Apache error log
echo "Received GET data: $DECODED" >&2

# HTTP response
echo "Content-Type: text/plain"
echo
echo "Data sent to terminal."
