#!/bin/sh

nginx -g "daemon off;" &
/usr/bin/LoadLinkerAgent
tail -f /dev/null
