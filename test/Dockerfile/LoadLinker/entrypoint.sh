#!/bin/sh
#
nginx -g "daemon off;" &
/usr/bin/LoadLinker
tail -f /dev/null
