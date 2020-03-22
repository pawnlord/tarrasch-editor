#!/bin/bash

gcc src/editor.c -o bin/edit.o

start_dir=$PWD
cd ~
if [ ! -d "$HOME/.tarrasch" ] ; then
	mkdir .tarrasch
fi

cd .tarrasch
if [ -d "$HOME/.tarrasch" ] ; then
  PATH="$PATH:$HOME/.tarrasch"
fi
touch syntax.cfg
cp $start_dir/syntaxfiles/syntax.cfg syntax.cfg
cd $start_dir