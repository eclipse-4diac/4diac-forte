
#include "ua_types_fordiacNamespace_generated.h"

/* DatatypeTest */
static UA_DataTypeMember DatatypeTest_members[3] = {
{
    UA_TYPENAME("Name") /* .memberName */
    UA_TYPES_STRING, /* .memberTypeIndex */
    0, /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("Age") /* .memberName */
    UA_TYPES_SBYTE, /* .memberTypeIndex */
    offsetof(UA_DatatypeTest, age) - offsetof(UA_DatatypeTest, name) - sizeof(UA_String), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},
{
    UA_TYPENAME("IsRegistered") /* .memberName */
    UA_TYPES_BOOLEAN, /* .memberTypeIndex */
    offsetof(UA_DatatypeTest, isRegistered) - offsetof(UA_DatatypeTest, age) - sizeof(UA_SByte), /* .padding */
    true, /* .namespaceZero */
    false /* .isArray */
},};
const UA_DataType UA_UA_TYPES_FORDIACNAMESPACE[UA_UA_TYPES_FORDIACNAMESPACE_COUNT] = {
/* DatatypeTest */
{
    UA_TYPENAME("DatatypeTest") /* .typeName */
    {2, UA_NODEIDTYPE_NUMERIC, {13}}, /* .typeId */
    sizeof(UA_DatatypeTest), /* .memSize */
    UA_UA_TYPES_FORDIACNAMESPACE_DATATYPETEST, /* .typeIndex */
    UA_DATATYPEKIND_STRUCTURE, /* .typeKind */
    false, /* .pointerFree */
    false, /* .overlayable */
    3, /* .membersSize */
    14, /* .binaryEncodingId */
    DatatypeTest_members /* .members */
},
};

