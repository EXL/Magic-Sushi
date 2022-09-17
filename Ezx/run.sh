#!/bin/sh
CURRENT_PATH=${0%/*}

. /home/native/.profile

LIBDIR1=/ezxlocal/download/mystuff/games/lib
LIBDIR2=/mmc/mmca1/games/lib
LIBDIR3=/mmc/mmca1/.Games/.lib
LIBDIR4=/mmc/mmca1/.system/lib
LIBDIR5=$mypath/lib

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$LIBDIR1:$LIBDIR2:$LIBDIR3:$LIBDIR4::$LIBDIR5
export HOME=$CURRENT_PATH
# export SDL_QT_INVERT_ROTATION=1
sleep 1

cd $CURRENT_PATH
exec $CURRENT_PATH/Magic-Sushi-EZX
