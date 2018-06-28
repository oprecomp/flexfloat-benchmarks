#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
ORIGIN=$DIR/..
cd $ORIGIN
python compile.py $DIR/config_file.txt $DIR/ >/dev/null 2>&1
cp $ORIGIN/adult.data $DIR
cd $DIR
./knn_flex $1

