
#if 1
# include "open62541.h"
#else
# include "ua_types_encoding_binary.h"
# include "ua_types_fordiacNamespace_generated.h"
#endif



/* DatatypeTest */
static UA_INLINE size_t
UA_DatatypeTest_calcSizeBinary(const UA_DatatypeTest *src) {
    return UA_calcSizeBinary(src, &UA_UA_TYPES_FORDIACNAMESPACE[UA_UA_TYPES_FORDIACNAMESPACE_DATATYPETEST]);
}
static UA_INLINE UA_StatusCode
UA_DatatypeTest_encodeBinary(const UA_DatatypeTest *src, UA_Byte **bufPos, const UA_Byte *bufEnd) {
    return UA_encodeBinary(src, &UA_UA_TYPES_FORDIACNAMESPACE[UA_UA_TYPES_FORDIACNAMESPACE_DATATYPETEST], bufPos, &bufEnd, nullptr, nullptr);
}
static UA_INLINE UA_StatusCode
UA_DatatypeTest_decodeBinary(const UA_ByteString *src, size_t *offset, UA_DatatypeTest *dst) {
    return UA_decodeBinary(src, offset, dst, &UA_UA_TYPES_FORDIACNAMESPACE[UA_UA_TYPES_FORDIACNAMESPACE_DATATYPETEST], nullptr);
}
