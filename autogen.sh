#!/bin/sh

if [ ! -d "config" ]; then
	mkdir config
fi

autoreconf --force --install -Iconfig
