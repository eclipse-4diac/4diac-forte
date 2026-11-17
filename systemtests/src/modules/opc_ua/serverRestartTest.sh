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
# An OPC UA client has two SUBSCRIBE FBs on one server, so one subscription holds two monitored items. The server
# is killed and started again right away. While the client is reset, it connects to the new server and open62541
# drops the old subscription. The client must survive this and get values from the new server.
#
# usage: serverRestartTest.sh <forte_systemtest> <server.fboot> <client.fboot>

FORTE="$1"
SERVER_BOOT="$2"
CLIENT_BOOT="$3"
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

sleep 4                      # the client is subscribed and gets values, but not as many as it needs to end
kill -9 "$SERVER_PID"
wait "$SERVER_PID" 2>/dev/null
"$FORTE" -f "$SERVER_BOOT" -c localhost:61499 >"$WORK_DIR/restartedServer.log" 2>&1 &
SERVER_PID=$!

WAITED=0
while kill -0 "$CLIENT_PID" 2>/dev/null && [ "$WAITED" -lt 60 ]; do
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
  ! grep -q "TEST_CONDITION_FAILED" "$WORK_DIR/client.log" &&
  ! grep -q "==ERROR" "$WORK_DIR/client.log"; then
  RESULT=0
fi

if [ "$RESULT" -ne 0 ]; then
  echo "client exit status: $CLIENT_EXIT, waited $WAITED s"
  echo "--- client log ---"
  tail -n 80 "$WORK_DIR/client.log"
fi
rm -rf "$WORK_DIR"
exit $RESULT
