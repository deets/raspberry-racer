if [ `uname` == "Darwin" ]; then
    GYP=/Users/deets/software/vc/gyp/gyp
    PROJECT_DIR=`dirname $0`
else
    GYP=gyp
    PROJECT_DIR=/home/deets/projects/raspberry-racer/cpp
fi

echo "project-dir: $PROJECT_DIR"

GYP_GENERATORS=ninja $GYP rracer.gyp --toplevel-dir=$PROJECT_DIR --depth=0 -Dlibrary=static_library
