#!/bin/sh

egrep '[^a-zA-Z]_' *.h | grep -v WIN32
