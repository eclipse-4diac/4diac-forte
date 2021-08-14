#/bin/bash

OPEN62541_FOLDER=$1
NAMESPACE=$2

python ${OPEN62541_FOLDER}tools/nodeset_compiler/nodeset_compiler.py \
   -e ${OPEN62541_FOLDER}deps/ua-nodeset/Schema/Opc.Ua.NodeSet2.xml \
   -x origin/fordiacNamespace.NodeSet2.xml \
   generated/ua_namespace_fordiacNamespace
    
python ${OPEN62541_FOLDER}tools/generate_datatypes.py --namespace=${NAMESPACE} \
   -c origin/fordiacNamespace.NodeIds.csv \
   -t origin/fordiacNamespace.Types.bsd \
   --no-builtin \
   generated/ua_types_fordiacNamespace

OS="$(uname -s)"


# Change header file
for file in generated/*.h
do
   if [[ "$OS" == "Darwin"* ]]
   then
     sed -i '' 's/#ifdef UA_ENABLE_AMALGAMATION/#if 1/g' $file
   else
     sed -i 's/#ifdef UA_ENABLE_AMALGAMATION/#if 1/g' $file
   fi
done




# Remove private information
for file in generated/*
do
  if [[ "$OS" == "Darwin"* ]]
  then
    sed -i '' '/Generated from/d' $file
    sed -i '' '/* on host/d' $file
  else
    sed -i '/Generated from/d' $file
    sed -i '/* on host/d' $file
  fi
done