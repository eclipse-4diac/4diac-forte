#!/bin/bash
cd ../.git/hooks
ln -s -f ../../buildsupport/post-receive.githook ./post-receive
cd ../../buildsupport