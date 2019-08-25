/*******************************************************************************
 * Copyright (c) 2010-2013 fortiss, TU Wien ACIN and others.
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
 *******************************************************************************/
#ifndef _FBDKASN1LAYER_H_
#define _FBDKASN1LAYER_H_

#include "comlayer.h"

// serialize includes
#include "../datatypes/forte_any.h"

class CIEC_TIME;
class CIEC_STRUCT;
class CIEC_ARRAY;
class CIEC_STRING;
class CIEC_WSTRING;

namespace forte {
  namespace com_infra {

    class CFBDKASN1ComLayer : public CComLayer{
      public:
        CFBDKASN1ComLayer(CComLayer* pa_poUpperLayer, CBaseCommFB * pa_poFB);
        virtual ~CFBDKASN1ComLayer();

        EComResponse sendData(void *pa_pvData, unsigned int pa_unSize); // top interface, called from top
        EComResponse recvData(const void *pa_pvData, unsigned int pa_unSize); // bottom interface, called from bottom

        /*!\brief Serialize an array of IEC data points into a byte array
         *
         * @param pa_pcBytes destination array for the serialization
         * @param pa_nStreamSize size of the destination array
         * @param pa_apoData  array of IEC data points
         * @param pa_nDataNum length of the data point array
         * @return on success the number of bytes written into the destination array,
         *         -1 on error.
         */
        static int serializeDataPointArray(TForteByte* pa_pcBytes, unsigned int pa_nStreamSize, TConstIEC_ANYPtr *pa_apoData, unsigned int pa_nDataNum);
        /*!\brief Serialize one IEC data point into a byte array
         *
         * @param pa_pcBytes destination array for the serialization
         * @param pa_nStreamSize size of the destination array
         * @param pa_roCIECData IEC data point
         * @return on success the number of bytes written into the destination array,
         *         -1 on error.
         */
        static int serializeDataPoint(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ANY &pa_roCIECData);


        /*! \brief Serialization of the data value according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *
         *  This function checks if enough bytes are available in the array.
         *
         * @param pa_pcBytes destination array for the serialization
         * @param pa_nStreamSize size of the destination array
         * @param pa_roCIECData IEC data point
         * @return on success the number of bytes written into the destination array,
         *         -1 on error.
         */
        static int serializeValue(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ANY &pa_roCIECData);

        /*!\brief Deserialize an array of IEC data points from a byte array
         *
         * @param pa_pcBytes target array for the deserialization
         * @param pa_nStreamSize size of the target array
         * @param pa_apoData  array of IEC data points
         * @param pa_nDataNum length of the data point array
         * @return true on success
         */
        static bool deserializeDataPointArray(const TForteByte* pa_pcBytes, unsigned int pa_nStreamSize, TIEC_ANYPtr *pa_apoData, unsigned int pa_nDataNum);

        /*!\brief Deserialize an array of IEC data points from a byte array
         *
         * @param pa_pcBytes target array for the deserialization
         * @param pa_nStreamSize size of the target array
         * @param pa_roCIECData  array of IEC data points
         * @return number of bytes used from the target array
         *         -1 on a general error, -2 if the tag does not fit
         */
        static int deserializeDataPoint(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ANY &pa_roCIECData);

        /*! \brief Deserialization of the data value according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *
         * @param pa_pcBytes target array for the deserialization
         * @param pa_nStreamSize size of the target array
         * @param pa_roCIECData  array of IEC data points
         * @return number of bytes used from the target array
         *         -1 on a general error, -2 if the tag does not fit
         */
        static int deserializeValue(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ANY &pa_roCIECData);

        enum EDataTypeTags{
          e_ANY_TAG = 0, e_BOOL_TAG = 1, e_SINT_TAG = 2, e_INT_TAG = 3, e_DINT_TAG = 4, e_LINT_TAG = 5, e_USINT_TAG = 6, e_UINT_TAG = 7, e_UDINT_TAG = 8, e_ULINT_TAG = 9, e_REAL_TAG = 10, e_LREAL_TAG = 11, e_TIME_TAG = 12, e_DATE_TAG = 13, e_TIME_OF_DAY_TAG = 14, e_DATE_AND_TIME_TAG = 15, e_STRING_TAG = 16, e_BYTE_TAG = 17, e_WORD_TAG = 18, e_DWORD_TAG = 19, e_LWORD_TAG = 20, e_WSTRING_TAG = 21, e_DerivedData_TAG = 26, e_DirectlyDerivedData_TAG = 27, e_EnumeratedData_TAG = 28, e_SubrangeData_TAG = 29, e_ARRAY_TAG = 22, //according to the compliance profile
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

        static const TForteByte scm_nNull = 5;
        const static TForteByte csm_aDataTags[][2];

        /*!\brief Serialize the Null tag into a byte array
         *
         * This operation will always take one byte
         * @param pa_pcBytes destination array for the serialization
         */
        static void serializeNull(TForteByte* pa_pcBytes){
          *pa_pcBytes = scm_nNull;
        }

        static bool isNull(const TForteByte* pa_pcBytes){
          return *pa_pcBytes == scm_nNull;
        }

        /*!\brief Serialize an array of IEC data points from a FB into a byte array
         *
         *
         * @param pa_pcBytes destination array for the serialization
         * @param pa_nStreamSize size of the destination array
         * @param pa_apoData  array of IEC data points
         * @param pa_nDataNum length of the data point array
         * @return on success the number of bytes written into the destination array,
         *         -1 on error.
         */
        static int serializeFBDataPointArray(TForteByte* pa_pcBytes, unsigned int pa_nStreamSize, TConstIEC_ANYPtr pa_aoData, unsigned int pa_nDataNum);


        /*! \brief Serialization of data tag  according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *
         * This operation will always take one byte.
         * @param pa_pcBytes destination array for the serialization
         * @param pa_roCIECData IEC data point
         */
        static void serializeTag(TForteByte* pa_pcBytes, const CIEC_ANY &pa_roCIECData);
        /** Specialization of the serialization of the data value for different data types. The interface behaves as
         *  described for static int serializeValue(TForteByte* pa_pcBytes, int pa_nStreamSize, TConstIEC_ANYPtr pa_pCIECData)
         * @{*/
        static int serializeValueSimpleDataType(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ANY & pa_roDataPoint);
        static int serializeValueTime(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_TIME & pa_roTime);
        static int serializeValueString(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_STRING & pa_roString);
#ifdef FORTE_USE_WSTRING_DATATYPE
        static int serializeValueWString(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_WSTRING & pa_roWString);
#endif //FORTE_USE_WSTRING_DATATYPE
        static int serializeValueStruct(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_STRUCT & pa_roWString);
#ifdef FORTE_SUPPORT_ARRAYS
        static int serializeArray(TForteByte* pa_pcBytes, int pa_nStreamSize, const CIEC_ARRAY &pa_roArray);
#endif //FORTE_SUPPORT_ARRAYS
        /**@}*/



        /*! \brief Deserialization of data tag  according to IEC 61499 Compliance Profile for
         *   Feasibility Demonstrations based on ISO/IEC 8825 (ASN.1).
         *   \param pa_cByte byte that contains the received tag.
         *   \param pa_roCIECData IEC data point
         *   \return true if the tag matched the data point
         */
        static bool deserializeTag(const TForteByte pa_cByte, CIEC_ANY &pa_roCIECData);


        /** Specialization of the deserialization of the data value for different data types. The interface behaves as
         *  described for static int deserializeValue(const TForteByte* pa_pcBytes, int pa_nStreamSize, TIEC_ANYPtr pa_pCIECData)
         * @{*/
        static int deserializeValueSimpleDataType(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ANY &pa_roIECData);
        static int deserializeValueTime(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_TIME &pa_roIECData);
#ifdef FORTE_USE_WSTRING_DATATYPE
        static int deserializeValueWString(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_WSTRING &pa_roIECData);
#endif //FORTE_USE_WSTRING_DATATYPE
        static int deserializeValueString(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_STRING &pa_roIECData);
#ifdef FORTE_SUPPORT_ARRAYS
        static int deserializeArray(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ARRAY &pa_roArray);
        static int deserializeValueBoolArray(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_ARRAY &pa_roArray, TForteUInt16 pa_unDecodedArraySize);
#endif //FORTE_SUPPORT_ARRAYS
        static int deserializeValueStruct(const TForteByte* pa_pcBytes, int pa_nStreamSize, CIEC_STRUCT &pa_roIECData);
        /**@}*/

        static unsigned int getRequiredSerializationSize(const CIEC_ANY &pa_roCIECData);

        EComResponse openConnection(char *pa_acLayerParameter);
        void closeConnection();
        void resizeDeserBuffer(unsigned int pa_size);


        TForteByte *mStatSerBuf;
        TForteUInt32 mStatSerBufSize;

        TForteByte *mDeserBuf;
        TForteUInt32 mDeserBufSize;
        unsigned int mDeserBufPos;

        TForteByte mDIPos;
        TForteByte mDOPos;
    };

  }
}

#endif
