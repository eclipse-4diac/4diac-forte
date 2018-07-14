/*******************************************************************************
 * Copyright (c) 2018 TU Vienna/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Martin Melik Merkumians
 *      - initial implementation of template metaprograms
 *******************************************************************************/

#ifndef SRC_CORE_IEC61131_CAST_HELPER_CPP_
#define SRC_CORE_IEC61131_CAST_HELPER_CPP_

class CIEC_BOOL;
class CIEC_BYTE;
class CIEC_WORD;
class CIEC_DWORD;
class CIEC_LWORD;
class CIEC_USINT;
class CIEC_UINT;
class CIEC_UDINT;
class CIEC_ULINT;
class CIEC_SINT;
class CIEC_INT;
class CIEC_DINT;
class CIEC_LINT;
class CIEC_REAL;
class CIEC_LREAL;

struct NullType{
    private: NullType();
};

/* invalid implicit casts */
template<typename T, typename U> struct implicit_cast {
    typedef NullType type;
};

/* Self-casts */
template<typename T> struct implicit_cast<T, T> {
    typedef T type;
};

/* BOOL casts */
template<> struct implicit_cast<CIEC_BOOL, CIEC_BYTE> {
    typedef CIEC_BYTE type;
};

template<> struct implicit_cast<CIEC_BOOL, CIEC_WORD> {
    typedef CIEC_WORD type;
};

template<> struct implicit_cast<CIEC_BOOL, CIEC_DWORD> {
    typedef CIEC_DWORD type;
};

template<> struct implicit_cast<CIEC_BOOL, CIEC_LWORD> {
    typedef CIEC_LWORD type;
};

/* BYTE casts */
template<> struct implicit_cast<CIEC_BYTE, CIEC_WORD> {
    typedef CIEC_WORD type;
};

template<> struct implicit_cast<CIEC_BYTE, CIEC_DWORD> {
    typedef CIEC_DWORD type;
};

template<> struct implicit_cast<CIEC_BYTE, CIEC_LWORD> {
    typedef CIEC_LWORD type;
};

/*WORD casts */
template<> struct implicit_cast<CIEC_WORD, CIEC_DWORD> {
    typedef CIEC_DWORD type;
};

template<> struct implicit_cast<CIEC_WORD, CIEC_LWORD> {
    typedef CIEC_LWORD type;
};
 /* DWORD casts */
template<> struct implicit_cast<CIEC_DWORD, CIEC_LWORD> {
    typedef CIEC_LWORD type;
};

/* USINT casts*/
template<> struct implicit_cast<CIEC_USINT, CIEC_UINT> {
    typedef CIEC_UINT type;
};

template<> struct implicit_cast<CIEC_USINT, CIEC_UDINT> {
    typedef CIEC_UDINT type;
};

template<> struct implicit_cast<CIEC_USINT, CIEC_ULINT> {
    typedef CIEC_ULINT type;
};

template<> struct implicit_cast<CIEC_USINT, CIEC_INT> {
    typedef CIEC_INT type;
};

template<> struct implicit_cast<CIEC_USINT, CIEC_DINT> {
    typedef CIEC_DINT type;
};

template<> struct implicit_cast<CIEC_USINT, CIEC_LINT> {
    typedef CIEC_LINT type;
};

#ifdef FORTE_USE_REAL_DATATYPE
template<> struct implicit_cast<CIEC_USINT, CIEC_REAL> {
    typedef CIEC_REAL type;
};

#ifdef FORTE_USE_64BIT_DATATYPES
template<> struct implicit_cast<CIEC_USINT, CIEC_LREAL> {
    typedef CIEC_LREAL type;
};
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

/* UINT casts*/
template<> struct implicit_cast<CIEC_UINT, CIEC_UDINT> {
    typedef CIEC_UDINT type;
};

template<> struct implicit_cast<CIEC_UINT, CIEC_ULINT> {
    typedef CIEC_ULINT type;
};

template<> struct implicit_cast<CIEC_UINT, CIEC_DINT> {
    typedef CIEC_DINT type;
};

template<> struct implicit_cast<CIEC_UINT, CIEC_LINT> {
    typedef CIEC_LINT type;
};

#ifdef FORTE_USE_REAL_DATATYPE
template<> struct implicit_cast<CIEC_UINT, CIEC_REAL> {
    typedef CIEC_REAL type;
};

#ifdef FORTE_USE_64BIT_DATATYPES
template<> struct implicit_cast<CIEC_UINT, CIEC_LREAL> {
    typedef CIEC_LREAL type;
};
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

/* UDINT casts*/
template<> struct implicit_cast<CIEC_UDINT, CIEC_ULINT> {
    typedef CIEC_ULINT type;
};

template<> struct implicit_cast<CIEC_UDINT, CIEC_LINT> {
    typedef CIEC_LINT type;
};

#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
template<> struct implicit_cast<CIEC_UDINT, CIEC_LREAL> {
    typedef CIEC_LREAL type;
};
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE


/* SINT casts */
template<> struct implicit_cast<CIEC_SINT, CIEC_INT> {
    typedef CIEC_INT type;
};

template<> struct implicit_cast<CIEC_SINT, CIEC_DINT> {
    typedef CIEC_DINT type;
};

template<> struct implicit_cast<CIEC_SINT, CIEC_LINT> {
    typedef CIEC_LINT type;
};

#ifdef FORTE_USE_REAL_DATATYPE
template<> struct implicit_cast<CIEC_SINT, CIEC_REAL> {
    typedef CIEC_REAL type;
};

#ifdef FORTE_USE_64BIT_DATATYPES
template<> struct implicit_cast<CIEC_SINT, CIEC_LREAL> {
    typedef CIEC_LREAL type;
};
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

/* INT casts */
template<> struct implicit_cast<CIEC_INT, CIEC_DINT> {
    typedef CIEC_DINT type;
};

template<> struct implicit_cast<CIEC_INT, CIEC_LINT> {
    typedef CIEC_LINT type;
};

#ifdef FORTE_USE_REAL_DATATYPE
template<> struct implicit_cast<CIEC_INT, CIEC_REAL> {
    typedef CIEC_REAL type;
};

#ifdef FORTE_USE_64BIT_DATATYPES
template<> struct implicit_cast<CIEC_INT, CIEC_LREAL> {
    typedef CIEC_LREAL type;
};
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

/* DINT casts */
template<> struct implicit_cast<CIEC_DINT, CIEC_LINT> {
    typedef CIEC_LINT type;
};

#ifdef FORTE_USE_REAL_DATATYPE
#ifdef FORTE_USE_64BIT_DATATYPES
template<> struct implicit_cast<CIEC_DINT, CIEC_LREAL> {
    typedef CIEC_LREAL type;
};
#endif //FORTE_USE_64BIT_DATATYPES
#endif //FORTE_USE_REAL_DATATYPE

#if __cplusplus >= 201103L //stdc11
template<typename CommonSubtype, typename T, typename U> struct are_of_subtype {
  enum {
    value = (std::is_base_of<CommonSubtype, T>::value && std::is_base_of<CommonSubtype, U>::value)
  };
};

template<typename T, typename U> struct get_castable_type{
    typedef typename std::conditional<std::is_same<NullType, typename implicit_cast<T, U>::type>::value, typename implicit_cast<U, T>::type, typename implicit_cast<T, U>::type >::type type;
};

#endif // __cplusplus >= 201103L //stdc11

#endif /* SRC_CORE_IEC61131_CAST_HELPER_CPP_ */

