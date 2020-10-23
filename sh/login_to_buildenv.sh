#!/bin/bash

# Copyright (c) 2018, 2019, 2020 chain Alliance.
# Permission to use, copy, modify, and/or distribute this software for any

cd `dirname $0`
workdir=`pwd`
project_path=$workdir
if [ ! -d $workdir/src ]; then
    cd ..
    project_path=`pwd`
    cd $workdir
fi

cid_file=".mtchain_builder.id" # file used to save container id
if [ -f $cid_file ]; then
    cid=`cat $cid_file`
else
    cid=""
fi

if [ -n $cid ]; then
    r=`docker ps -f id=$cid -a -q | wc -l` # if the container has existed
    if [ $r -gt 0 ]; then
        r=`docker ps -f id=$cid -q | wc -l` # if the container is running
        if [ $r -le 0 ]; then
            docker start $cid
        fi

        docker exec -it $cid /bin/bash
        exit 0
    fi
fi

image_name="mtchaind_build"
exist_docker_image=`docker image ls -q $image_name | wc -l`
if [ $exist_docker_image -lt 1 ]; then
    echo "docker image '$image_name' doesn't exit!"
# TODO: first, try to download it from git repositories and load it into local repository

# if failed to download it directly, try to build it from scratch
    echo "start building image and it may be take some time, please be patient to wait ..."
    sleep 1
    docker build -t $image_name -f Dockerfile.compile .
fi

columns=`tput cols`
lines=`tput lines`
cid=`docker run -t -d -v$project_path:$project_path -w $project_path -e COLUMNS=$columns -e LINES=$lines -e TERM=xterm-color -v /var/run/docker.sock:/var/run/docker.sock --privileged $image_name`
echo $cid > $cid_file
docker exec -it $cid /bin/bash
