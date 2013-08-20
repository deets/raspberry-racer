if [ `uname` == Darwin ]; then
    NINJA=/usr/local/bin/ninja
    $NINJA -C out/Default/ -v test mac_rracer
else
    NINJA=/home/deets/software/vc/ninja/ninja 
    $NINJA -C out/Default/ -v test rracer
fi


