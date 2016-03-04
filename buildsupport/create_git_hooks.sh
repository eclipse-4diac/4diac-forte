#!/bin/bash
cd ../.git/hooks
ln -s -f ../../buildsupport/post_receive.githook ./post_receive
cd ../../buildsupport