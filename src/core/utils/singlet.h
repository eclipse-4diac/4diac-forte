/*******************************************************************************
 * Copyright (c) 2005 - 2010 ACIN
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alois Zoitl
 *    - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _SINGLET_H_
#define _SINGLET_H_

/*!\ingroup CORE \brief Macro for providing singleton functionality.
 *
 * Classes derived from this template have only one single instance.
 * Usage: in the class declaration:
 *      class CMyNewClass{
 *        SINGLETON(TypeName)
 *
 * As the macro already defines a default constructor you have to implement it
 * in the cpp file.
 */
#define DECLARE_SINGLETON(TypeName)                                   \
  public:                                                             \
    static TypeName &getInstance(void);                               \
  private:                                                            \
    TypeName();                                                       \
    TypeName(const TypeName&);                                         \
    TypeName& operator = (const TypeName &);                          \
    ~TypeName();

#define DEFINE_SINGLETON(TypeName)                                    \
 TypeName& TypeName::getInstance(void){                               \
   static TypeName oInstance;                                                \
   return oInstance;                                                  \
 };

#endif /*SINGLET_H_*/
