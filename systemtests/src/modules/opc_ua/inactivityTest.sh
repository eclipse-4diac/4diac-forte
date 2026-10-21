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
# An OPC UA client subscribes to a server that is then stopped (SIGSTOP). The TCP connection stays open, but the
# server does not send anything. After the client noticed the inactivity of its subscription it has to connect
# again and subscribe again, so that it receives values once the server runs again.
#
# usage: inactivityTest.sh <forte_systemtest> <server.fboot> <client.fboot>

FORTE="$1"
SERVER_BOOT="$2"
CLIENT_BOOT="$3"
WORK_DIR=$(mktemp -d)
SERVER_PID=""
CLIENT_PID=""

cleanup() {
  [ -n "$SERVER_PID" ] && kill -CONT "$SERVER_PID" 2>/dev/null
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

sleep 6                      # the client is subscribed and gets values
kill -STOP "$SERVER_PID"     # the server is silent, the connection stays open
sleep 16                     # longer than two inactivity reports of the subscription (about 6 s each)
kill -CONT "$SERVER_PID"

# the client ends itself when it got enough values, which is only possible after it subscribed again
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
  grep -q "is inactive" "$WORK_DIR/client.log"; then
  RESULT=0
fi

if [ "$RESULT" -ne 0 ]; then
  echo "client exit status: $CLIENT_EXIT, waited $WAITED s"
  echo "--- client log ---"
  tail -n 60 "$WORK_DIR/client.log"
  echo "--- server log ---"
  tail -n 20 "$WORK_DIR/server.log"
fi
rm -rf "$WORK_DIR"
exit $RESULT
