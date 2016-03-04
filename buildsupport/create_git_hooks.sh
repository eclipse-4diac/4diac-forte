#!/bin/bash
cd ../.git/hooks
ln -s ../../buildsupport/post_receive.githook ./post_receive
cd ../../buildsupport