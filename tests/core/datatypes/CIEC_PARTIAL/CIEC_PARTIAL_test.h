/*******************************************************************************
 * Copyright (c) 2011 - 2012 ACIN, nxtControl, 2018 TU Vienna/ACIN
 *               2020 Johannes Kepler University Linz
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Martin Melik Merkumians, Ingo Hegny, Alois Zoitl, Stanislav Meduna - initial API and implementation and/or initial documentation
 *   Ernst Blecha - Adds partial access tests
 *******************************************************************************/

#ifndef CIEC_ANY_BIT_PARTIAL_TEST
#define CIEC_ANY_BIT_PARTIAL_TEST

#define test4X(var, offset, b3, b2, b1, b0) \
  BOOST_CHECK_EQUAL((var.partial<CIEC_BOOL,0+offset>()),b0);\
  BOOST_CHECK_EQUAL((var.partial<CIEC_BOOL,1+offset>()),b1);\
  BOOST_CHECK_EQUAL((var.partial<CIEC_BOOL,2+offset>()),b2);\
  BOOST_CHECK_EQUAL((var.partial<CIEC_BOOL,3+offset>()),b3);

#define test4X_0(var, offset) test4X(var, offset, 0, 0, 0, 0)
#define test4X_1(var, offset) test4X(var, offset, 0, 0, 0, 1)
#define test4X_2(var, offset) test4X(var, offset, 0, 0, 1, 0)
#define test4X_3(var, offset) test4X(var, offset, 0, 0, 1, 1)
#define test4X_4(var, offset) test4X(var, offset, 0, 1, 0, 0)
#define test4X_5(var, offset) test4X(var, offset, 0, 1, 0, 1)
#define test4X_6(var, offset) test4X(var, offset, 0, 1, 1, 0)
#define test4X_7(var, offset) test4X(var, offset, 0, 1, 1, 1)
#define test4X_8(var, offset) test4X(var, offset, 1, 0, 0, 0)
#define test4X_9(var, offset) test4X(var, offset, 1, 0, 0, 1)
#define test4X_A(var, offset) test4X(var, offset, 1, 0, 1, 0)
#define test4X_B(var, offset) test4X(var, offset, 1, 0, 1, 1)
#define test4X_C(var, offset) test4X(var, offset, 1, 1, 0, 0)
#define test4X_D(var, offset) test4X(var, offset, 1, 1, 0, 1)
#define test4X_E(var, offset) test4X(var, offset, 1, 1, 1, 0)
#define test4X_F(var, offset) test4X(var, offset, 1, 1, 1, 1)


#define set4X(var, offset, b3, b2, b1, b0) \
  var.partial<CIEC_BOOL,0+offset>()=b0;\
  var.partial<CIEC_BOOL,1+offset>()=b1;\
  var.partial<CIEC_BOOL,2+offset>()=b2;\
  var.partial<CIEC_BOOL,3+offset>()=b3;

#define set4X_0(var, offset) set4X(var, offset, 0, 0, 0, 0)
#define set4X_1(var, offset) set4X(var, offset, 0, 0, 0, 1)
#define set4X_2(var, offset) set4X(var, offset, 0, 0, 1, 0)
#define set4X_3(var, offset) set4X(var, offset, 0, 0, 1, 1)
#define set4X_4(var, offset) set4X(var, offset, 0, 1, 0, 0)
#define set4X_5(var, offset) set4X(var, offset, 0, 1, 0, 1)
#define set4X_6(var, offset) set4X(var, offset, 0, 1, 1, 0)
#define set4X_7(var, offset) set4X(var, offset, 0, 1, 1, 1)
#define set4X_8(var, offset) set4X(var, offset, 1, 0, 0, 0)
#define set4X_9(var, offset) set4X(var, offset, 1, 0, 0, 1)
#define set4X_A(var, offset) set4X(var, offset, 1, 0, 1, 0)
#define set4X_B(var, offset) set4X(var, offset, 1, 0, 1, 1)
#define set4X_C(var, offset) set4X(var, offset, 1, 1, 0, 0)
#define set4X_D(var, offset) set4X(var, offset, 1, 1, 0, 1)
#define set4X_E(var, offset) set4X(var, offset, 1, 1, 1, 0)
#define set4X_F(var, offset) set4X(var, offset, 1, 1, 1, 1)

#endif
