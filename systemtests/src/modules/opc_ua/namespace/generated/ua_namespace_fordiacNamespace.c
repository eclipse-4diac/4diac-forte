/* WARNING: This is a generated file.
 * Any manual changes will be overwritten. */

#include "ua_namespace_fordiacNamespace.h"

/* DatatypeTest - ns=1;i=13 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_0_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_DataTypeAttributes attr = UA_DataTypeAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("", "DatatypeTest");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_DATATYPE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 13LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 22LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 45LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "DatatypeTest"), UA_NODEID_NULL,
      (const UA_NodeAttributes *) &attr, &UA_TYPES[UA_TYPES_DATATYPEATTRIBUTES], NULL, NULL);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_0_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 13LU));
}

/* Default JSON - ns=1;i=30 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_1_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_ObjectAttributes attr = UA_ObjectAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("", "Default JSON");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_OBJECT, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 30LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 0LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 0LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "Default JSON"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 76LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES], NULL, NULL);
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 30LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 38LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 13LU), false);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_1_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 30LU));
}

/* Default XML - ns=1;i=22 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_2_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_ObjectAttributes attr = UA_ObjectAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("", "Default XML");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_OBJECT, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 22LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 0LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 0LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "Default XML"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 76LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES], NULL, NULL);
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 22LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 38LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 13LU), false);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_2_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 22LU));
}

/* Default Binary - ns=1;i=14 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_3_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_ObjectAttributes attr = UA_ObjectAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("", "Default Binary");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_OBJECT, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 14LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 0LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 0LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "Default Binary"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 76LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES], NULL, NULL);
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 14LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 38LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 13LU), false);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_3_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 14LU));
}

/* fordiacNamespace - ns=1;i=23 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_4_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 15LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue =
      UA_STRING("<Value>"
                "<ByteString "
                "xmlns=\"http://opcfoundation.org/UA/2008/02/"
                "Types.xsd\">PHhzOnNjaGVtYQ0KICB4bWxuczp4cz0iaHR0cDovL3d3dy53My5vcmcvMjAwMS9YTUxTY2hlbWEi"
                "DQogIHhtbG5zOnVhPSJodHRwOi8vb3BjZm91bmRhdGlvbi5vcmcvVUEvMjAwOC8wMi9UeXBlcy54"
                "c2QiDQogIHhtbG5zOnRucz0iaHR0cHM6Ly9naXQuZWNsaXBzZS5vcmcvNGRpYWMvb3JnLmVjbGlw"
                "c2UuNGRpYWMiDQogIHRhcmdldE5hbWVzcGFjZT0iaHR0cHM6Ly9naXQuZWNsaXBzZS5vcmcvNGRp"
                "YWMvb3JnLmVjbGlwc2UuNGRpYWMiDQogIGVsZW1lbnRGb3JtRGVmYXVsdD0icXVhbGlmaWVkIg0K"
                "Pg0KICA8eHM6aW1wb3J0IG5hbWVzcGFjZT0iaHR0cDovL29wY2ZvdW5kYXRpb24ub3JnL1VBLzIw"
                "MDgvMDIvVHlwZXMueHNkIiAvPg0KDQogIDx4czpjb21wbGV4VHlwZSBuYW1lPSJEYXRhdHlwZVRl"
                "c3QiPg0KICAgIDx4czpzZXF1ZW5jZT4NCiAgICAgIDx4czplbGVtZW50IG5hbWU9Ik5hbWUiIHR5"
                "cGU9InhzOnN0cmluZyIgbWluT2NjdXJzPSIwIiBuaWxsYWJsZT0idHJ1ZSIgLz4NCiAgICAgIDx4"
                "czplbGVtZW50IG5hbWU9IkFnZSIgdHlwZT0ieHM6Ynl0ZSIgbWluT2NjdXJzPSIwIiAvPg0KICAg"
                "ICAgPHhzOmVsZW1lbnQgbmFtZT0iSXNSZWdpc3RlcmVkIiB0eXBlPSJ4czpib29sZWFuIiBtaW5P"
                "Y2N1cnM9IjAiIC8+DQogICAgPC94czpzZXF1ZW5jZT4NCiAgPC94czpjb21wbGV4VHlwZT4NCiAg"
                "PHhzOmVsZW1lbnQgbmFtZT0iRGF0YXR5cGVUZXN0IiB0eXBlPSJ0bnM6RGF0YXR5cGVUZXN0IiAv"
                "Pg0KDQogIDx4czpjb21wbGV4VHlwZSBuYW1lPSJMaXN0T2ZEYXRhdHlwZVRlc3QiPg0KICAgIDx4"
                "czpzZXF1ZW5jZT4NCiAgICAgIDx4czplbGVtZW50IG5hbWU9IkRhdGF0eXBlVGVzdCIgdHlwZT0i"
                "dG5zOkRhdGF0eXBlVGVzdCIgbWluT2NjdXJzPSIwIiBtYXhPY2N1cnM9InVuYm91bmRlZCIgbmls"
                "bGFibGU9InRydWUiIC8+DQogICAgPC94czpzZXF1ZW5jZT4NCiAgPC94czpjb21wbGV4VHlwZT4N"
                "CiAgPHhzOmVsZW1lbnQgbmFtZT0iTGlzdE9mRGF0YXR5cGVUZXN0IiB0eXBlPSJ0bnM6TGlzdE9m"
                "RGF0YXR5cGVUZXN0IiBuaWxsYWJsZT0idHJ1ZSI+PC94czplbGVtZW50Pg0KDQo8L3hzOnNjaGVt"
                "YT4=</ByteString>"
                "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "fordiacNamespace");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 23LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 92LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "fordiacNamespace"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 72LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_4_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 23LU));
}

/* DatatypeTest - ns=1;i=27 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_5_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 12LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING(
      "<Value>"
      "<String xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">//xs:element[@name='DatatypeTest']</String>"
      "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "DatatypeTest");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 27LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 23LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "DatatypeTest"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 69LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 27LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 39LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 22LU), false);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_5_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 27LU));
}

/* Deprecated - ns=1;i=26 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_6_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 1LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING("<Value>"
                                 "<Boolean xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">true</Boolean>"
                                 "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "Deprecated");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 26LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 23LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "Deprecated"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_6_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 26LU));
}

/* NamespaceUri - ns=1;i=25 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_7_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 12LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING(
      "<Value>"
      "<String "
      "xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">https://git.eclipse.org/4diac/org.eclipse.4diac</String>"
      "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 25LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 23LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "NamespaceUri"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_7_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 25LU));
}

/* fordiacNamespace - ns=1;i=15 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_8_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 15LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue =
      UA_STRING("<Value>"
                "<ByteString "
                "xmlns=\"http://opcfoundation.org/UA/2008/02/"
                "Types.xsd\">PG9wYzpUeXBlRGljdGlvbmFyeQ0KICB4bWxuczpvcGM9Imh0dHA6Ly9vcGNmb3VuZGF0aW9uLm9y"
                "Zy9CaW5hcnlTY2hlbWEvIg0KICB4bWxuczp4c2k9Imh0dHA6Ly93d3cudzMub3JnLzIwMDEvWE1M"
                "U2NoZW1hLWluc3RhbmNlIg0KICB4bWxuczp1YT0iaHR0cDovL29wY2ZvdW5kYXRpb24ub3JnL1VB"
                "LyINCiAgeG1sbnM6dG5zPSJodHRwczovL2dpdC5lY2xpcHNlLm9yZy80ZGlhYy9vcmcuZWNsaXBz"
                "ZS40ZGlhYyINCiAgRGVmYXVsdEJ5dGVPcmRlcj0iTGl0dGxlRW5kaWFuIg0KICBUYXJnZXROYW1l"
                "c3BhY2U9Imh0dHBzOi8vZ2l0LmVjbGlwc2Uub3JnLzRkaWFjL29yZy5lY2xpcHNlLjRkaWFjIg0K"
                "Pg0KICA8b3BjOkltcG9ydCBOYW1lc3BhY2U9Imh0dHA6Ly9vcGNmb3VuZGF0aW9uLm9yZy9VQS8i"
                "IExvY2F0aW9uPSJPcGMuVWEuQmluYXJ5U2NoZW1hLmJzZCIvPg0KDQogIDxvcGM6U3RydWN0dXJl"
                "ZFR5cGUgTmFtZT0iRGF0YXR5cGVUZXN0IiBCYXNlVHlwZT0idWE6RXh0ZW5zaW9uT2JqZWN0Ij4N"
                "CiAgICA8b3BjOkZpZWxkIE5hbWU9Ik5hbWUiIFR5cGVOYW1lPSJvcGM6U3RyaW5nIiAvPg0KICAg"
                "IDxvcGM6RmllbGQgTmFtZT0iQWdlIiBUeXBlTmFtZT0ib3BjOlNCeXRlIiAvPg0KICAgIDxvcGM6"
                "RmllbGQgTmFtZT0iSXNSZWdpc3RlcmVkIiBUeXBlTmFtZT0ib3BjOkJvb2xlYW4iIC8+DQogIDwv"
                "b3BjOlN0cnVjdHVyZWRUeXBlPg0KDQo8L29wYzpUeXBlRGljdGlvbmFyeT4=</ByteString>"
                "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "fordiacNamespace");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 15LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 93LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "fordiacNamespace"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 72LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_8_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 15LU));
}

/* DatatypeTest - ns=1;i=19 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_9_begin(UA_Server *server, UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 12LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING("<Value>"
                                 "<String xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">DatatypeTest</String>"
                                 "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "DatatypeTest");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 19LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 15LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "DatatypeTest"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 69LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 19LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 39LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 14LU), false);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_9_finish(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 19LU));
}

/* Deprecated - ns=1;i=18 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_10_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 1LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING("<Value>"
                                 "<Boolean xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">true</Boolean>"
                                 "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "Deprecated");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 18LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 15LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "Deprecated"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_10_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 18LU));
}

/* NamespaceUri - ns=1;i=17 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_11_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 12LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING(
      "<Value>"
      "<String "
      "xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">https://git.eclipse.org/4diac/org.eclipse.4diac</String>"
      "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "NamespaceUri");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 17LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 15LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "NamespaceUri"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_11_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 17LU));
}

/* TestObject - ns=1;i=1 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_12_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
  attr.displayName = UA_LOCALIZEDTEXT("", "TestObject");

#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS

  attr.description = UA_LOCALIZEDTEXT("", "Object to test in 4diac");

#endif

  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_OBJECTTYPE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 1LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 58LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 45LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "TestObject"), UA_NODEID_NULL,
      (const UA_NodeAttributes *) &attr, &UA_TYPES[UA_TYPES_OBJECTTYPEATTRIBUTES], NULL, NULL);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_12_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 1LU));
}

/* TestMethod2 - ns=1;i=7 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_13_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_MethodAttributes attr = UA_MethodAttributes_default;
  attr.executable = true;
  attr.userExecutable = true;
  attr.displayName = UA_LOCALIZEDTEXT("", "TestMethod2");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_METHOD, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 7LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 1LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "TestMethod2"), UA_NODEID_NULL,
      (const UA_NodeAttributes *) &attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES], NULL, NULL);
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 7LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_13_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addMethodNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 7LU),
                                        NULL, 0, NULL, 0, NULL);
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* OutputArguments - ns=1;i=9 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_14_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  attr.valueRank = 1;
  attr.arrayDimensionsSize = 1;
  UA_UInt32 arrayDimensions[1];
  arrayDimensions[0] = 0;
  attr.arrayDimensions = &arrayDimensions[0];
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 296LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING("<Value>"
                                 "<ListOfExtensionObject xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">"
                                 "<ExtensionObject>"
                                 "<TypeId>"
                                 "<Identifier>i=297</Identifier>"
                                 "</TypeId>"
                                 "<Body>"
                                 "<Argument>"
                                 "<Name>OutputArgument</Name>"
                                 "<DataType>"
                                 "<Identifier>i=6</Identifier>"
                                 "</DataType>"
                                 "<ValueRank>-1</ValueRank>"
                                 "<ArrayDimensions/>"
                                 "</Argument>"
                                 "</Body>"
                                 "</ExtensionObject>"
                                 "</ListOfExtensionObject>"
                                 "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
  if (UA_Variant_isScalar(&attr.value) && attr.value.data != NULL) {
    UA_LOG_WARNING(UA_Server_getConfig(server)->logging, UA_LOGCATEGORY_USERLAND,
                   "Node ns=1;i=9: ValueRank=1 but the XML value is scalar. Auto-wrapping into a one-element array.");
    attr.value.arrayLength = 1;
  }
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 9LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 7LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "OutputArguments"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 9LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_14_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 9LU));
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* InputArguments - ns=1;i=8 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_15_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  attr.valueRank = 1;
  attr.arrayDimensionsSize = 1;
  UA_UInt32 arrayDimensions[1];
  arrayDimensions[0] = 0;
  attr.arrayDimensions = &arrayDimensions[0];
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 296LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING("<Value>"
                                 "<ListOfExtensionObject xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">"
                                 "<ExtensionObject>"
                                 "<TypeId>"
                                 "<Identifier>i=297</Identifier>"
                                 "</TypeId>"
                                 "<Body>"
                                 "<Argument>"
                                 "<Name>InputArgument</Name>"
                                 "<DataType>"
                                 "<Identifier>i=6</Identifier>"
                                 "</DataType>"
                                 "<ValueRank>-1</ValueRank>"
                                 "<ArrayDimensions/>"
                                 "</Argument>"
                                 "</Body>"
                                 "</ExtensionObject>"
                                 "</ListOfExtensionObject>"
                                 "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
  if (UA_Variant_isScalar(&attr.value) && attr.value.data != NULL) {
    UA_LOG_WARNING(UA_Server_getConfig(server)->logging, UA_LOGCATEGORY_USERLAND,
                   "Node ns=1;i=8: ValueRank=1 but the XML value is scalar. Auto-wrapping into a one-element array.");
    attr.value.arrayLength = 1;
  }
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 8LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 7LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "InputArguments"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 8LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_15_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 8LU));
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* TestMethod - ns=1;i=4 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_16_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_MethodAttributes attr = UA_MethodAttributes_default;
  attr.executable = true;
  attr.userExecutable = true;
  attr.displayName = UA_LOCALIZEDTEXT("", "TestMethod");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_METHOD, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 4LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 1LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "TestMethod"), UA_NODEID_NULL,
      (const UA_NodeAttributes *) &attr, &UA_TYPES[UA_TYPES_METHODATTRIBUTES], NULL, NULL);
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 4LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_16_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addMethodNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 4LU),
                                        NULL, 0, NULL, 0, NULL);
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* OutputArguments - ns=1;i=6 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_17_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  attr.valueRank = 1;
  attr.arrayDimensionsSize = 1;
  UA_UInt32 arrayDimensions[1];
  arrayDimensions[0] = 0;
  attr.arrayDimensions = &arrayDimensions[0];
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 296LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING("<Value>"
                                 "<ListOfExtensionObject xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">"
                                 "<ExtensionObject>"
                                 "<TypeId>"
                                 "<Identifier>i=297</Identifier>"
                                 "</TypeId>"
                                 "<Body>"
                                 "<Argument>"
                                 "<Name>OutputArgument</Name>"
                                 "<DataType>"
                                 "<Identifier>i=6</Identifier>"
                                 "</DataType>"
                                 "<ValueRank>-1</ValueRank>"
                                 "<ArrayDimensions/>"
                                 "</Argument>"
                                 "</Body>"
                                 "</ExtensionObject>"
                                 "</ListOfExtensionObject>"
                                 "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
  if (UA_Variant_isScalar(&attr.value) && attr.value.data != NULL) {
    UA_LOG_WARNING(UA_Server_getConfig(server)->logging, UA_LOGCATEGORY_USERLAND,
                   "Node ns=1;i=6: ValueRank=1 but the XML value is scalar. Auto-wrapping into a one-element array.");
    attr.value.arrayLength = 1;
  }
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "OutputArguments");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 6LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 4LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "OutputArguments"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 6LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_17_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 6LU));
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* InputArguments - ns=1;i=5 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_18_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  attr.valueRank = 1;
  attr.arrayDimensionsSize = 1;
  UA_UInt32 arrayDimensions[1];
  arrayDimensions[0] = 0;
  attr.arrayDimensions = &arrayDimensions[0];
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 296LU);
#ifdef UA_ENABLE_XML_ENCODING
  UA_String xmlValue = UA_STRING("<Value>"
                                 "<ListOfExtensionObject xmlns=\"http://opcfoundation.org/UA/2008/02/Types.xsd\">"
                                 "<ExtensionObject>"
                                 "<TypeId>"
                                 "<Identifier>i=297</Identifier>"
                                 "</TypeId>"
                                 "<Body>"
                                 "<Argument>"
                                 "<Name>InputArgument</Name>"
                                 "<DataType>"
                                 "<Identifier>i=6</Identifier>"
                                 "</DataType>"
                                 "<ValueRank>-1</ValueRank>"
                                 "<ArrayDimensions/>"
                                 "</Argument>"
                                 "</Body>"
                                 "</ExtensionObject>"
                                 "</ListOfExtensionObject>"
                                 "</Value>");
  UA_DecodeXmlOptions opts;
  memset(&opts, 0, sizeof(UA_DecodeXmlOptions));
  opts.unwrapped = true;
  opts.namespaceMapping = nsMapping;
  opts.customTypes = UA_Server_getConfig(server)->customDataTypes;
  retVal |= UA_decodeXml(&xmlValue, &attr.value, &UA_TYPES[UA_TYPES_VARIANT], &opts);
  if (UA_Variant_isScalar(&attr.value) && attr.value.data != NULL) {
    UA_LOG_WARNING(UA_Server_getConfig(server)->logging, UA_LOGCATEGORY_USERLAND,
                   "Node ns=1;i=5: ValueRank=1 but the XML value is scalar. Auto-wrapping into a one-element array.");
    attr.value.arrayLength = 1;
  }
#endif /* UA_ENABLE_XML_ENCODING */
  attr.displayName = UA_LOCALIZEDTEXT("", "InputArguments");
  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 5LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 4LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 46LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 0), "InputArguments"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 68LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
#ifdef UA_ENABLE_XML_ENCODING
  UA_Variant_clear(&attr.value);
#endif /* UA_ENABLE_XML_ENCODING */
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 5LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_18_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
#ifdef UA_ENABLE_METHODCALLS
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 5LU));
#else
  return UA_STATUSCODE_GOOD;
#endif /* UA_ENABLE_METHODCALLS */
}

/* Variable2 - ns=1;i=3 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_19_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 6LU);
  attr.displayName = UA_LOCALIZEDTEXT("", "Variable2");

#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS

  attr.description = UA_LOCALIZEDTEXT("", "Variable 2");

#endif

  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 3LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 1LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "Variable2"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 63LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 3LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_19_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 3LU));
}

/* Variable1 - ns=1;i=2 */

static UA_StatusCode function_ua_namespace_fordiacNamespace_20_begin(UA_Server *server,
                                                                     UA_NamespaceMapping *nsMapping) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  UA_VariableAttributes attr = UA_VariableAttributes_default;
  attr.minimumSamplingInterval = 0.000000;
  attr.userAccessLevel = 1;
  attr.accessLevel = 1;
  /* Value rank inherited */
  attr.valueRank = -2;
  attr.dataType = UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 6LU);
  attr.displayName = UA_LOCALIZEDTEXT("", "Variable1");

#ifdef UA_ENABLE_NODESET_COMPILER_DESCRIPTIONS

  attr.description = UA_LOCALIZEDTEXT("", "Variable 1");

#endif

  retVal |= UA_Server_addNode_begin(
      server, UA_NODECLASS_VARIABLE, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 2LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 1LU),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 47LU),
      UA_QUALIFIEDNAME(UA_NamespaceMapping_local2Remote(nsMapping, 1), "Variable1"),
      UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 63LU), (const UA_NodeAttributes *) &attr,
      &UA_TYPES[UA_TYPES_VARIABLEATTRIBUTES], NULL, NULL);
  retVal |=
      UA_Server_addReference(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 2LU),
                             UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 37LU),
                             UA_EXPANDEDNODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 0), 78LU), true);
  return retVal;
}

static UA_StatusCode function_ua_namespace_fordiacNamespace_20_finish(UA_Server *server,
                                                                      UA_NamespaceMapping *nsMapping) {
  return UA_Server_addNode_finish(server, UA_NODEID_NUMERIC(UA_NamespaceMapping_local2Remote(nsMapping, 1), 2LU));
}

UA_StatusCode ua_namespace_fordiacNamespace(UA_Server *server) {
  UA_StatusCode retVal = UA_STATUSCODE_GOOD;
  /* Use namespace ids generated by the server */
  UA_UInt16 ns[3];
  ns[0] = UA_Server_addNamespace(server, "http://opcfoundation.org/UA/");
  ns[1] = UA_Server_addNamespace(server, "https://git.eclipse.org/4diac/org.eclipse.4diac");
  UA_UInt16 nsMappingTable[2] = {ns[0], ns[1]};
  UA_NamespaceMapping nsMapping;
  memset(&nsMapping, 0, sizeof(UA_NamespaceMapping));
  nsMapping.local2remote = ns;
  nsMapping.local2remoteSize = 2;
  nsMapping.remote2local = nsMappingTable;
  nsMapping.remote2localSize = 2;

  /* Load custom datatype definitions into the server */
  retVal |= function_ua_namespace_fordiacNamespace_0_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_1_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_2_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_3_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_4_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_5_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_6_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_7_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_8_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_9_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_10_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_11_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_12_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_13_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_14_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_15_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_16_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_17_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_18_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_19_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_20_begin(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_20_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_19_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_18_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_17_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_16_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_15_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_14_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_13_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_12_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_11_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_10_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_9_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_8_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_7_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_6_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_5_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_4_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_3_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_2_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_1_finish(server, &nsMapping);
  retVal |= function_ua_namespace_fordiacNamespace_0_finish(server, &nsMapping);
  return retVal;
}
