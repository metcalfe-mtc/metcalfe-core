#!/bin/bash

echo "------------------------- daemon process start -------------------------"
trap "echo 'receive stop signal!'; ./mtchaind stop; exit" TERM INT
trap "echo '------------------------- daemon process end -------------------------'" EXIT
echo $0

if [ $# -gt 0 ]; then
    workdir=$1
else
    workdir=`echo $0 | awk '{ n = split($1, arr, "/");  s = ""; for (i = 1; i < n; ++i) { s = s "" arr[i] "/" } print(s) }'`
fi

if [ "`pwd`" != "$workdir" ]; then
    echo "change working directory"
    echo -n "from: "
    pwd
    cd $workdir
    echo -n "to:   "
    pwd
fi

echo

if [ ! -d "log" ]; then
    mkdir -p log
fi

if [ -d "db" ]; then
    load_flag="--load"
else
    load_flag="--start"
fi

count=1
while true
do

echo "start mtchaind[$count] ..."
./mtchaind --fg $load_flag >> debug.log &
wait $!
ret=$?
delay=1
echo -e "$ret\n"

if [ -f "debug.log" ]; then
    now=`date "+%Y%m%d%H%M%S"`
    log_file="debug_${now}.log"
    echo $log_file

    mv debug.log log/$log_file
fi

if [ $ret -eq 0 ]; then
    load_flag="--load"
    count=1
elif [ $ret -eq 134 ]; then # mtchaind has been started alone
    ./mtchaind stop
    echo
else
    let ++count
    if [ $count -gt 10 ]; then
        load_flag="--start"
    fi
    delay=6
fi

sleep $delay
done
