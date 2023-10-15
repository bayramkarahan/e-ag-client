#!/bin/bash

sudo iptables -F
for entry in $(cat /usr/share/e-ag/webblocklist)
do
 # echo "$entry"
sudo iptables -A OUTPUT -p tcp -m string --string "$entry" --algo kmp -j REJECT
done

