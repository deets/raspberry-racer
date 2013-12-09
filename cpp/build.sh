configuration=Debug
if [ `uname` == Darwin ]; then
    NINJA=/usr/local/bin/ninja
    $NINJA -C out/$configuration/ -v box2dlib test mac_rracer
else
    NINJA=/home/deets/software/vc/ninja/ninja 
    $NINJA -C out/$configuration/ -v box2dlib test rracer
fi


