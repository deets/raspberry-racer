if [ `uname` == Darwin ]; then
    NINJA=/usr/local/bin/ninja
else
    NINJA=/home/deets/software/vc/ninja/ninja 
fi

$NINJA -C out/Default/ -v $1