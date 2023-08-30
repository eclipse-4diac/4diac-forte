/*******************************************************************************
 * Copyright (c) 2010-2013 fortiss, TU Wien ACIN and others.
 *               2023 Martin Erich Jobst
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *    Alois Zoitl - initial implementation and rework communication infrastructure, bug fixes
 *    Michael Hofmann - option for custom datatypes send by fbdkasn1layer
 *    Monika Wenger - rename datatype files to forte_datatype
 *    Ingo Hegny - serialize/deserialize for STRUCT, changed datatype for IP-communication
 *    Stanislav Meduna - make serializeNull and isNull public
 *    Martin Jobst - account for new FB layout and varying data type size
 *******************************************************************************/
#ifndef _FBDKASN1LAYER_H_
#define _FBDKASN1LAYER_H_

#include "comlayer.h"

// serialize includes
#include "../datatypes/forte_any.h"

#include <set>

class CIEC_TIME;
class CIEC_STRUCT;
class CIEC_ARRAY;
class CIEC_STRING;
class CIEC_WSTRING;

namespace forte {
  namespace com_infra {

    class CFBDKASN1ComLayer : public CComLayer{
      public:
        CFBDKASN1ComLayer(CComLayer* paUpperLayer, CBaseCommFB * paFB);
        ~CFBDKASN1ComLayer() override;

        EComResponse sendData(void *paData, unsigned int paSize) override; // top interface, called from top
        EComResponse recvData(const void *paData, unsigned int paSize) override; // bottom interface, called from bottom

        /*!\brief Serialize an array of IEC data points into a byte array
         *
         * @param paBytes destination array for the serialization
         * @param paStreamSize size of the destination array
         * @param paData  array of IEC data points
         * @param paDataNum length of the data point array
         * @return on success the number of bytes written into the destination array,
         *         -1 on error.
         */
        static int serializeDataPointArray(TForteByte* paBytes, const size_t paStreamSize, TConstIEC_ANYPtr *paData, size_t paDataNum);
        /*!\brief Serialize one IEC data point into a byte array
         *
         * @param paBytes destination array for the serialization
         * @param paStreamSize size of the destination array
         * @param paCIECData IEC data point
         * @return on success the number of bytes written into the destination array,
         *         -1 on error.
         */
        static int serializeDataPoint(TForteByte* paBytes, int paStreamSize, const CIEC_ANY &paCIECData);

        /*! \brief Serialization of the data value according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *
         *  This function checks if enough bytes are available in the array.
         *
         * @param paBytes destination array for the serialization
         * @param paStreamSize size of the destination array
         * @param paCIECData IEC data point
         * @return on success the number of bytes written into the destination array,
         *         -1 on error.
         */
        static int serializeValue(TForteByte* paBytes, int paStreamSize, const CIEC_ANY &paCIECData);

        /*!\brief Deserialize an array of IEC data points from a byte array
         *
         * @param paBytes target array for the deserialization
         * @param paStreamSize size of the target array
         * @param paData  array of IEC data points
         * @param paDataNum length of the data point array
         * @return true on success
         */
        static bool deserializeDataPointArray(const TForteByte* paBytes, unsigned int paStreamSize, TIEC_ANYPtr *paData, size_t paDataNum);

        /*!\brief Deserialize an array of IEC data points from a byte array
         *
         * @param paBytes target array for the deserialization
         * @param paStreamSize size of the target array
         * @param paCIECData  array of IEC data points
         * @return number of bytes used from the target array
         *         -1 on a general error, -2 if the tag does not fit
         */
        static int deserializeDataPoint(const TForteByte* paBytes, int paStreamSize, CIEC_ANY &paCIECData);

        /*! \brief Deserialization of the data value according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *
         * @param paBytes target array for the deserialization
         * @param paStreamSize size of the target array
         * @param paCIECData  array of IEC data points
         * @return number of bytes used from the target array
         *         -1 on a general error, -2 if the tag does not fit
         */
        static int deserializeValue(const TForteByte* paBytes, int paStreamSize, CIEC_ANY &paCIECData);

        enum EDataTypeTags {
          e_ANY_TAG = 0, e_BOOL_TAG = 1, e_SINT_TAG = 2, e_INT_TAG = 3, e_DINT_TAG = 4, e_LINT_TAG = 5, e_USINT_TAG = 6, e_UINT_TAG = 7, e_UDINT_TAG = 8, e_ULINT_TAG = 9, e_REAL_TAG = 10, e_LREAL_TAG = 11, e_TIME_TAG = 12, e_DATE_TAG = 13, e_TIME_OF_DAY_TAG = 14, e_DATE_AND_TIME_TAG = 15, e_STRING_TAG = 16, e_BYTE_TAG = 17, e_WORD_TAG = 18, e_DWORD_TAG = 19, e_LWORD_TAG = 20, e_WSTRING_TAG = 21, e_DerivedData_TAG = 26, e_DirectlyDerivedData_TAG = 27, e_EnumeratedData_TAG = 28, e_SubrangeData_TAG = 29, e_ARRAY_TAG = 22, //according to the compliance profile
          e_CHAR_TAG = 23, e_WCHAR_TAG = 24, e_LDATE_TAG = 25, e_LTIME_OF_DAY_TAG = 26, e_LDATE_AND_TIME_TAG = 27, e_LTIME_TAG = 28,
          e_STRUCT_TAG = 31
        };
        /*! \ingroup COREDTS\brief ASN.1 tag classes
         */
        enum EASN1TagClass{
          e_UNIVERSAL = 0, e_APPLICATION = 64, e_CONTEXT = 128, e_PRIVATE = 192
        };
        /*! \ingroup COREDTS\brief ASN.1 encoding types
         */
        enum EASN1EncodingType{
          e_PRIMITIVE = 0, e_CONSTRUCTED = 32
        };


      protected:
      private:

        static const TForteByte scmNull = 5;
        static const TForteByte csmDataTags[][2];

        static const std::set<CIEC_ANY::EDataTypeID> scmSimpleEncodableDataTypes;

        /*!\brief Serialize the Null tag into a byte array
         *
         * This operation will always take one byte
         * @param paBytes destination array for the serialization
         */
        static void serializeNull(TForteByte* paBytes){
          *paBytes = scmNull;
        }

        static bool isNull(const TForteByte* paBytes){
          return *paBytes == scmNull;
        }

        /*! \brief Serialization of data tag  according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *
         * This operation will always take one byte.
         * @param paBytes destination array for the serialization
         * @param paCIECData IEC data point
         */
        static void serializeTag(TForteByte* paBytes, const CIEC_ANY &paCIECData);
        /** Specialization of the serialization of the data value for different data types. The interface behaves as
         *  described for static int serializeValue(TForteByte* paBytes, int paStreamSize, TConstIEC_ANYPtr paCIECData)
         * @{*/
        static int serializeValueSimpleDataType(TForteByte* paBytes, int paStreamSize, const CIEC_ANY & paDataPoint);
        static int serializeValueTime(TForteByte* paBytes, int paStreamSize, const CIEC_TIME & paTime);
        static int serializeValueString(TForteByte* paBytes, int paStreamSize, const CIEC_STRING & paString);
#ifdef FORTE_USE_WSTRING_DATATYPE
        static int serializeValueWString(TForteByte* paBytes, int paStreamSize, const CIEC_WSTRING & paWString);
#endif //FORTE_USE_WSTRING_DATATYPE
        static int serializeValueStruct(TForteByte* paBytes, int paStreamSize, const CIEC_STRUCT & paWString);
        static int serializeArray(TForteByte *paBytes, int paStreamSize, const CIEC_ARRAY &paArray);
        /**@}*/



        /*! \brief Deserialization of data tag  according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *   \param paByte byte that contains the received tag.
         *   \param paCIECData IEC data point
         *   \return true if the tag matched the data point
         */
        static bool deserializeTag(const TForteByte paByte, CIEC_ANY &paCIECData);


        /** Specialization of the deserialization of the data value for different data types. The interface behaves as
         *  described for static int deserializeValue(const TForteByte* paBytes, int paStreamSize, TIEC_ANYPtr paCIECData)
         * @{*/
        static int deserializeValueSimpleDataType(const TForteByte* paBytes, int paStreamSize, CIEC_ANY &paIECData);
        static int deserializeValueTime(const TForteByte* paBytes, int paStreamSize, CIEC_TIME &paIECData);
#ifdef FORTE_USE_WSTRING_DATATYPE
        static int deserializeValueWString(const TForteByte* paBytes, int paStreamSize, CIEC_WSTRING &paIECData);
#endif //FORTE_USE_WSTRING_DATATYPE
        static int deserializeValueString(const TForteByte* paBytes, int paStreamSize, CIEC_STRING &paIECData);
        static int deserializeArray(const TForteByte *paBytes, int paStreamSize, CIEC_ARRAY &paArray);
        static int deserializeValueBoolArray(const TForteByte *paBytes, int paStreamSize, CIEC_ARRAY &paArray, TForteUInt16 paDecodedArraySize);
        static int deserializeValueStruct(const TForteByte* paBytes, int paStreamSize, CIEC_STRUCT &paIECData);
        /**@}*/

        static size_t getRequiredSerializationSize(const CIEC_ANY &paCIECData);

        EComResponse openConnection(char *paLayerParameter) override;
        void closeConnection() override;
        void resizeDeserBuffer(unsigned int pa_size);


        TForteByte *mStatSerBuf;
        TForteUInt32 mStatSerBufSize;

        TForteByte *mDeserBuf;
        TForteUInt32 mDeserBufSize;
        unsigned int mDeserBufPos;

        TPortId mDIPos;
        TPortId mDOPos;
    };

  }
}

#endif
