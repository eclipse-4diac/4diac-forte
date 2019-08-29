/*******************************************************************************
 * Copyright (c) 2012 AIT
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Werner Tremmel - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef VARIANTNEW_H_
#define VARIANTNEW_H_

#include <boost/lexical_cast.hpp>

/*! \brief structure for simplified handling of the VARIANT
 *
 *  The structure simplifies the handling of the VARIANT with numeric types.
 */

#include <OleAuto.h>
#include <comutil.h>

struct Variant : public VARIANT{
  public:
    class VariantException : public std::exception{
      public:
        VariantException(){
        }
        virtual const char* what() throw (){
          return "VariantException: unknown variant type";
        }
    };

    ~Variant(){
    }
    Variant(){
    }

    explicit Variant(const _variant_t &value) :
        VARIANT(value){
    }
    explicit Variant(const VARIANT &value) :
        VARIANT(value){
    }
    explicit Variant(bool value){
      setbool(value);
    }
    explicit Variant(BOOL value){
      setbool(value != 0);
    }
    explicit Variant(VARIANT_BOOL value){
      setbool(value != 0);
    }

    template<typename T> explicit Variant(T value){
      set<T>(value);
    }

    template<typename T> T get() const{
      switch (vt){
        case VT_I8:
          return static_cast < T > (llVal);
        case VT_I4:
          return static_cast < T > (lVal);
        case VT_I2:
          return static_cast < T > (iVal);
        case VT_I1:
          return static_cast < T > (cVal);
        case VT_UI8:
          return static_cast < T > (ullVal);
        case VT_UI4:
          return static_cast < T > (ulVal);
        case VT_UI2:
          return static_cast < T > (uiVal);
        case VT_UI1:
          return static_cast < T > (bVal);
        case VT_R8:
          return static_cast < T > (dblVal);
        case VT_R4:
          return static_cast < T > (fltVal);
        case VT_INT:
          return static_cast < T > (intVal);
        case VT_UINT:
          return static_cast < T > (uintVal);
        case VT_BOOL:
          return boolVal != 0 ? static_cast < T > (1) : static_cast < T > (0);

        default:
          throw VariantException();
      }
    }
    template<> std::string get<std::string>() const{
      switch (vt){
        case VT_I8:
          return boost::lexical_cast < std::string > (llVal);
        case VT_I4:
          return boost::lexical_cast < std::string > (lVal);
        case VT_I2:
          return boost::lexical_cast < std::string > (iVal);
        case VT_I1:
          return boost::lexical_cast < std::string > (cVal);
        case VT_UI8:
          return boost::lexical_cast < std::string > (ullVal);
        case VT_UI4:
          return boost::lexical_cast < std::string > (ulVal);
        case VT_UI2:
          return boost::lexical_cast < std::string > (uiVal);
        case VT_UI1:
          return boost::lexical_cast < std::string > (bVal);
        case VT_R8:
          return boost::lexical_cast < std::string > (dblVal);
        case VT_R4:
          return boost::lexical_cast < std::string > (fltVal);
        case VT_INT:
          return boost::lexical_cast < std::string > (intVal);
        case VT_UINT:
          return boost::lexical_cast < std::string > (uintVal);
        case VT_BOOL:
          return boolVal != 0 ? boost::lexical_cast < std::string > (true) : boost::lexical_cast < std::string > (false);

        default:
          throw VariantException();
      }
    }
    template<> bool get<bool>() const{
      switch (vt){
        case VT_I8:
          return llVal != 0 ? true : false;
        case VT_I4:
          return lVal != 0 ? true : false;
        case VT_I2:
          return iVal != 0 ? true : false;
        case VT_I1:
          return cVal != 0 ? true : false;
        case VT_UI8:
          return ullVal != 0 ? true : false;
        case VT_UI4:
         return ulVal != 0 ? true : false;
        case VT_UI2:
         return uiVal != 0 ? true : false;
        case VT_UI1:
        return bVal != 0 ? true : false;
        case VT_R8:
        return dblVal != 0 ? true : false;
        case VT_R4:
         return fltVal != 0 ? true : false;
        case VT_INT:
         return intVal != 0 ? true : false;
        case VT_UINT:
          return uintVal != 0 ? true : false;
        case VT_BOOL:
          return boolVal != 0 ? true : false;

        default:
          throw VariantException();
      }
    }
    void setbool(bool value){
      boolVal = value ? 0xffff : 0;
      vt = VT_BOOL;
    }
    template<typename T> void set(T value){
      getField<T>() = value;
      vt = getType<T>();
    }
    template<> void set<bool>(bool value){
      setbool(value);
    }
    void setBOOL(BOOL value){
      setbool(value != 0);
    }
    void setVARIANT_BOOL(VARIANT_BOOL value){
      setbool(value != 0);
    }

    template<typename T> bool isType() const{
      return vt == getType<T>();
    }

    bool isInteger() const{
      switch (vt){
        case VT_I8:
        case VT_I4:
        case VT_I2:
        case VT_I1:
        case VT_INT:
          return true;
        default:
          return false;
      }
    }
    bool isUnsignedInteger() const{
      switch (vt){
        case VT_UI8:
        case VT_UI4:
        case VT_UI2:
        case VT_UI1:
        case VT_UINT:
          return true;
        default:
          return false;
      }
    }
    bool isFloatingPoint() const{
      switch (vt){
        case VT_R8:
        case VT_R4:
          return true;
        default:
          return false;
      }
    }

    template<typename T> VARTYPE getType() const{
      constraint<T>();
    }
    template<> VARTYPE getType<LONGLONG>() const{
      return VT_I8;
    }
    template<> VARTYPE getType<LONG>() const{
      return VT_I4;
    }
    template<> VARTYPE getType<SHORT>() const{
      return VT_I2;
    }
    template<> VARTYPE getType<CHAR>() const{
      return VT_I1;
    }
    template<> VARTYPE getType<signed char>() const{
      return VT_I1;
    }
    template<> VARTYPE getType<ULONGLONG>() const{
      return VT_UI8;
    }
    template<> VARTYPE getType<ULONG>() const{
      return VT_UI4;
    }
    template<> VARTYPE getType<USHORT>() const{
      return VT_UI2;
    }
    template<> VARTYPE getType<BYTE>() const{
      return VT_UI1;
    }
    template<> VARTYPE getType<float>() const{
      return VT_R4;
    }
    template<> VARTYPE getType<double>() const{
      return VT_R8;
    }
    //template<> VARTYPE getType<BOOL>() const{
    //  return VT_BOOL;
    //}
    template<> VARTYPE getType<INT>() const{
      return VT_INT;
    }
    template<> VARTYPE getType<UINT>() const{
      return VT_UINT;
    }

    template<typename T> T &getField(){
      constraint<T>();
    }
    template<> LONGLONG &getField<LONGLONG>(){
      return llVal;
    }
    template<> LONG &getField<LONG>(){
      return lVal;
    }
    template<> SHORT &getField<SHORT>(){
      return iVal;
    }
    template<> CHAR &getField<CHAR>(){
      return cVal;
    }
    template<> signed char &getField<signed char>(){
      return (signed char&)cVal;
    }
    template<> ULONGLONG &getField<ULONGLONG>(){
      return ullVal;
    }
    template<> ULONG &getField<ULONG>(){
      return ulVal;
    }
    template<> USHORT &getField<USHORT>(){
      return uiVal;
    }
    template<> BYTE &getField<BYTE>(){
      return bVal;
    }
    template<> float &getField<float>(){
      return fltVal;
    }
    template<> double &getField<double>(){
      return dblVal;
    }
    template<> INT &getField<INT>(){
      return intVal;
    }
    template<> UINT &getField<UINT>(){
      return uintVal;
    }

    bool operator==(const Variant &other) const{
      if(vt != other.vt) {
        return false;
      }
      ULONGLONG mask = ~0ul;
      switch (vt){
        case VT_I4:
        case VT_UI4:
        case VT_INT:
        case VT_UINT:
        case VT_R4:
          mask = 0xffffffffull;
          break;
        case VT_I2:
        case VT_UI2:
        case VT_BOOL:
          mask = 0xffffull;
          break;
        case VT_I1:
        case VT_UI1:
          mask = 0xffull;
          break;
      }
      return ((ullVal ^ other.ullVal) & mask) == 0;
    }

    /*!  \brief Compiler error, when used
     *
     *  This is only here to throw a compiler error when used. So the unsupported types are caught by the compiler
     *  and not by the runtime system
     */
    template<typename T> static void constraint(){
      char a[2];
      a[1.2] = 0;
    }
};

#endif //VARIANTNEW_H_
