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
# An OPC UA client calls a method of a server, the method has no NodeId in the ID of the FBs. The server is then
# replaced by one that creates another node before the method, so the method gets another NodeId. After the client
# connected again it has to find the method by its browse path and call it again.
#
# usage: callMethodRestartTest.sh <forte_systemtest> <server.fboot> <restartedServer.fboot> <client.fboot>

FORTE="$1"
SERVER_BOOT="$2"
RESTARTED_SERVER_BOOT="$3"
CLIENT_BOOT="$4"
WORK_DIR=$(mktemp -d)
SERVER_PID=""
CLIENT_PID=""

cleanup() {
  [ -n "$SERVER_PID" ] && kill "$SERVER_PID" 2>/dev/null
  [ -n "$CLIENT_PID" ] && kill "$CLIENT_PID" 2>/dev/null
  wait 2>/dev/null
}
trap cleanup EXIT

"$FORTE" -f "$SERVER_BOOT" -c localhost:61499 >"$WORK_DIR/server.log" 2>&1 &
SERVER_PID=$!
sleep 3
"$FORTE" -f "$CLIENT_BOOT" -c localhost:61500 >"$WORK_DIR/client.log" 2>&1 &
CLIENT_PID=$!

# the client prints CALLS_DONE for every successful call: restart the server after the first one, so that the
# client has used the NodeId of the first server and most calls have to go to the restarted one
WAITED=0
while ! grep -q "CALLS_DONE = " "$WORK_DIR/client.log" && [ "$WAITED" -lt 300 ]; do
  sleep 0.1
  WAITED=$((WAITED + 1))
done
if ! grep -q "CALLS_DONE = " "$WORK_DIR/client.log"; then
  echo "the client did not call the method of the first server"
  echo "--- client log ---"
  tail -n 60 "$WORK_DIR/client.log"
  rm -rf "$WORK_DIR"
  exit 1
fi
kill -9 "$SERVER_PID"
wait "$SERVER_PID" 2>/dev/null
"$FORTE" -f "$RESTARTED_SERVER_BOOT" -c localhost:61499 >"$WORK_DIR/restartedServer.log" 2>&1 &
SERVER_PID=$!

# the client ends itself when it called the method often enough, which is only possible after the restart
WAITED=0
while kill -0 "$CLIENT_PID" 2>/dev/null && [ "$WAITED" -lt 60 ]; do
  sleep 1
  WAITED=$((WAITED + 1))
done

CLIENT_STATUS="not ended"
if ! kill -0 "$CLIENT_PID" 2>/dev/null; then
  wait "$CLIENT_PID"
  CLIENT_STATUS=$?
  CLIENT_PID=""
fi

RESULT=1
if [ "$CLIENT_STATUS" = "0" ] &&
  grep -q "TEST_CONDITION_PASSED" "$WORK_DIR/client.log" &&
  ! grep -q "TEST_CONDITION_FAILED" "$WORK_DIR/client.log"; then
  RESULT=0
fi

if [ "$RESULT" -ne 0 ]; then
  echo "client exit status: $CLIENT_STATUS, waited $WAITED s"
  echo "--- client log ---"
  tail -n 60 "$WORK_DIR/client.log"
  echo "--- restarted server log ---"
  tail -n 20 "$WORK_DIR/restartedServer.log"
fi
rm -rf "$WORK_DIR"
exit $RESULT
