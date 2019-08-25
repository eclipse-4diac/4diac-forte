/*******************************************************************************
 * Copyright (c) 2016 Herwig Eichler, www.conmeleon.org
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Herwig Eichler  - initial API and implementation and initial documentation
 *******************************************************************************/

#ifndef UTIL_UNCOPYABLE_H_
#define UTIL_UNCOPYABLE_H_

/*! \class CUncopyable
 *  \brief CUncopyable is a base class for objects which are unique and should not be copied
 *
 * That's why the copy constructor and the assignment operator are private and not implemented, to prevent the compiler from creating it's own.
 * The default constructor and the destructor are protected to allow derived classes implement their own constructors and destructors.
 * Derived classes should be derived privately.
 */
class CUncopyable {
protected:
  CUncopyable() {}
  ~CUncopyable() {}
private:
  CUncopyable (const CUncopyable&);
  CUncopyable& operator = (const CUncopyable&);
};

#endif /* UTIL_UNCOPYABLE_H_ */
