#!/bin/sh
#*******************************************************************************
# Copyright (c) 2026 HR Agrartechnik GmbH
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Franz Höpfinger - initial implementation
#*******************************************************************************
# An OPC UA client has two clients: one subscribes to a counter of server A, the other one calls a method of
# server B once. The method client is the last one in the iteration list and has nothing pending once its call is
# answered. The client must keep getting values of the counter afterwards.
#
# usage: iterationTest.sh <forte_systemtest> <counterServer.fboot> <methodServer.fboot> <client.fboot>

FORTE="$1"
COUNTER_SERVER_BOOT="$2"
METHOD_SERVER_BOOT="$3"
CLIENT_BOOT="$4"
WORK_DIR=$(mktemp -d)
COUNTER_SERVER_PID=""
METHOD_SERVER_PID=""
CLIENT_PID=""

cleanup() {
  [ -n "$COUNTER_SERVER_PID" ] && kill -9 "$COUNTER_SERVER_PID" 2>/dev/null
  [ -n "$METHOD_SERVER_PID" ] && kill -9 "$METHOD_SERVER_PID" 2>/dev/null
  [ -n "$CLIENT_PID" ] && kill -9 "$CLIENT_PID" 2>/dev/null # a stuck client does not end on SIGTERM
  wait 2>/dev/null
}
trap cleanup EXIT

"$FORTE" -f "$COUNTER_SERVER_BOOT" -c localhost:61499 >"$WORK_DIR/counterServer.log" 2>&1 &
COUNTER_SERVER_PID=$!
"$FORTE" -f "$METHOD_SERVER_BOOT" -c localhost:61501 -op 4841 >"$WORK_DIR/methodServer.log" 2>&1 &
METHOD_SERVER_PID=$!
sleep 3
"$FORTE" -f "$CLIENT_BOOT" -c localhost:61500 >"$WORK_DIR/client.log" 2>&1 &
CLIENT_PID=$!

# the client ends itself when it got enough values
WAITED=0
while kill -0 "$CLIENT_PID" 2>/dev/null && [ "$WAITED" -lt 40 ]; do
  sleep 1
  WAITED=$((WAITED + 1))
done

CLIENT_EXIT=timeout
if ! kill -0 "$CLIENT_PID" 2>/dev/null; then
  wait "$CLIENT_PID"
  CLIENT_EXIT=$?
fi

RESULT=1
if [ "$CLIENT_EXIT" = 0 ] &&
  grep -q "TEST_CONDITION_PASSED" "$WORK_DIR/client.log" &&
  ! grep -q "TEST_CONDITION_FAILED" "$WORK_DIR/client.log"; then
  RESULT=0
fi

if [ "$RESULT" -ne 0 ]; then
  echo "client exit status: $CLIENT_EXIT, waited $WAITED s"
  echo "--- client log ---"
  tail -n 60 "$WORK_DIR/client.log"
fi
rm -rf "$WORK_DIR"
exit $RESULT
