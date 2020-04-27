#!/bin/bash
install -Dm4754 -o root -g smartctl smartctl-sticky /usr/local/bin/smartctl-sticky
if [[ ! -e /etc/smartctl-sticky.conf ]]; then
    install -m644 -o root -g root smartctl-sticky.conf /etc/
fi
