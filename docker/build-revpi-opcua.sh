#!/usr/bin/env bash
#*******************************************************************************
# Copyright (c) 2026 UniversalAutomation.org AISBL
#
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v2.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v20.html
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Israilov Sardor
#*******************************************************************************
# Build the 4diac FORTE runtime image with Revolution Pi IO *and* OPC UA (FORTE_COM_OPC_UA)
# for arm64, for publishing IO values via PUBLISH_1 "opc_ua[WRITE;...]" FBs.
#
# Usage:
#   OPEN62541_SOURCE_DIR=/path/to/open62541 ./docker/build-revpi-opcua.sh
#   TAG=myregistry/forte:revpi-opcua ./docker/build-revpi-opcua.sh --push
#   PLATFORM=linux/amd64 ./docker/build-revpi-opcua.sh   # compile check on an amd64 host
#
# Requires an open62541 checkout at the v1.5.4 tag (pinned by com/opc_ua/CMakeLists.txt's
# find_package(open62541 1.5.4 REQUIRED), the same version CI builds) with the deps/ua-nodeset
# submodule initialized:
#   cd "$OPEN62541_SOURCE_DIR" && git checkout v1.5.4 && git submodule update --init deps/ua-nodeset
# It is passed to Docker as a second build context (buildx --build-context), not copied into the
# FORTE repo or committed; see docker/Dockerfile.revpi-opcua for how it's built and linked in.
#
# Notes:
# - Cross-building arm64 from an amd64 host needs QEMU binfmt handlers and a docker-container
#   buildx builder (docker/setup-qemu-action + docker/setup-buildx-action in CI, or `docker buildx
#   create --use` locally). Building natively on the RevPi needs neither.
# - Run with /dev/piControl0 passed through (docker run --device /dev/piControl0) and reach port
#   4840 (OPC UA TCP) in addition to 61499.
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="${FORTE_SOURCE_DIR:-$(cd "$script_dir/.." && pwd)}"
if [[ ! -f "$repo_root/CMakeLists.txt" ]]; then
  echo "ERROR: '$repo_root' is not the 4diac FORTE repository root (set FORTE_SOURCE_DIR)." >&2
  exit 2
fi

dockerfile="$repo_root/docker/Dockerfile.revpi-opcua"
if [[ ! -f "$dockerfile" ]]; then
  echo "ERROR: Dockerfile not found: $dockerfile" >&2
  exit 2
fi

open62541_dir="${OPEN62541_SOURCE_DIR:-}"
if [[ -z "$open62541_dir" ]]; then
  echo "ERROR: Set OPEN62541_SOURCE_DIR to a local open62541 checkout (tag v1.5.4, deps/ua-nodeset submodule initialized)." >&2
  exit 2
fi
if [[ ! -f "$open62541_dir/CMakeLists.txt" ]]; then
  echo "ERROR: '$open62541_dir' is not an open62541 checkout (no CMakeLists.txt)." >&2
  exit 2
fi

platform="${PLATFORM:-linux/arm64}"
tag="${TAG:-4diac-forte:revpi-opcua-arm64}"
push_flag=""
for arg in "$@"; do
  case "$arg" in
    --push) push_flag="--push" ;;
  esac
done
load_flag="--load"
if [[ -n "$push_flag" ]]; then
  load_flag=""
fi

docker buildx build \
  --platform "$platform" \
  --file "$dockerfile" \
  --build-context "open62541=$open62541_dir" \
  --tag "$tag" \
  $load_flag $push_flag \
  "$repo_root"

echo "Built $tag for $platform"
