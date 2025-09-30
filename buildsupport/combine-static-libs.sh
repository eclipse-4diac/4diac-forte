#!/usr/bin/env bash

#############################################################################
# Copyright (c) 2025 Martin Erich Jobst
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#    Martin Erich Jobst
#      - initial API and implementation and/or initial documentation
#############################################################################

for lib in libforte*.a; do
  if [ ! -e "${lib}" ]; then
    echo "No libforte*.a found in the current working directory"
    exit 1
  fi
  break
done

ar -M - <<-EOF
CREATE libforte-all.a
$(for lib in libforte*.a; do
  if [[ "${lib}" != "libforte-all.a" ]]; then
    echo ADDLIB "${lib}"
  fi
done)
SAVE
END
EOF
