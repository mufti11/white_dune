#!/bin/sh

egrep '[^a-zA-Z]_' *.cpp | grep -v WIN32
