#!/bin/bash

IP="127.0.0.1"

if [ "${1}" != "" ]
then
   IP=${1}
fi 

echo "Using ip ${IP}"

ids=$(curl -X GET "http://${IP}:8444/authorization/mgmt/intracloud" -s | grep id\" | tr -s ' ' | cut -d ':' -f2 | cut -d ',' -f1)

counter=0
for i in $ids
do
  if ! (( ${counter} % 4 )) 
  then
    echo "Deleting authorization intracloud entry with id=$i"
    curl -X DELETE "http://${IP}:8444/authorization/mgmt/intracloud/$i" -H  "accept: application/json"
  fi
  counter=$(( ${counter} + 1))
done


ids=$(curl -X GET "http://${IP}:8440/orchestrator/mgmt/store/all" -H  "accept: application/json" -s | grep id\" | tr -s ' ' | cut -d ':' -f2 | cut -d ',' -f1)

counter=0
for i in $ids
do

  if (( ${counter} == 2 ))
  then
    echo "Deleting orchestration rule of consumer with id=$i"
    curl -X DELETE "http://${IP}:8440/orchestrator/mgmt/store/consumerId/$i" -H  "accept: application/json"
    counter=$(( ${counter} + 1))
  elif (( ${counter} == 3 ))
  then
    counter=0
  else
    counter=$(( ${counter} + 1))
  fi
done

