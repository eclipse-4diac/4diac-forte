/*******************************************************************************
 * Copyright (c) 2011 - 2013 ACIN, fortiss GmbH
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _STATICASSERT_H_
#define _STATICASSERT_H_

#ifdef __GNUC__
  //The latest GCC is rather picky about unused typedefs. We need this for our static assert and its ok there that it is unused there.
  #define FORTE_ATTRIBUTE_UNUSED __attribute__((unused))
#else
  #define FORTE_ATTRIBUTE_UNUSED
#endif

/*! A simple compile time assert which is based on the books:
 *    - Modern C++ Design, Andrei Alexandrescu
 *    - Imperfect C++, Matthew Wilson
 *  It tries to combine the simplicity of Wilson's solution with the
 *  better compiler message generation of Alexandrescu's solution.
 */
#define FORTE_STATIC_ASSERT(paExpr, paMsg) \
  { typedef int ERROR_##paMsg[(paExpr) ? 1 : -1] FORTE_ATTRIBUTE_UNUSED; }

//(void)sizeof(ERROR_##paMsg);}

#endif
