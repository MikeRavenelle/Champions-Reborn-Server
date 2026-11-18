#!/bin/sh
set -e
[ -f config.ini ] || cp /opt/config.default.ini config.ini
[ -z "$PUBLIC_IP" ] || sed -i "s|^public_ip=.*|public_ip=$PUBLIC_IP|" config.ini
exec champions-server
