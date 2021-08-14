#/bin/bash

# Compile the file fordiacTest.xml to create all the Nodeset files (.NodeSet2.xml, .csv, bsd)

docker run --mount type=bind,source=$(pwd),target=/model ua-modelcompiler -- -console -d2 /model/fordiacTests.xml -cg /model/fordiacNamespace.NodeIds.csv -o2 /model
