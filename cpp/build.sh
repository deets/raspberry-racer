configuration=Debug
if [ `uname` == Darwin ]; then
    export PATH=/usr/local/bin:$PATH
    NINJA=/usr/local/bin/ninja
    $NINJA -C out/$configuration/ -v box2dlib test mac-rracer openvg-playground
else
    NINJA=/home/deets/software/vc/ninja/ninja 
    $NINJA -C out/$configuration/ -v box2dlib test rracer
fi


