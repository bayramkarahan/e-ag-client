#!/bin/bash
myuser=""
mydisplay=""
##myuser=$(cat /run/systemd/sessions/$(loginctl list-sessions|grep "seat"|awk '{print $1}')|grep "USER"|awk -F '=' '{print $2}')
##mydisplay=$(cat /run/systemd/sessions/$(loginctl list-sessions|grep "seat"|awk '{print $1}')|grep "DISPLAY"|awk -F '=:' '{print $2}')

for ids in $(loginctl list-sessions|grep "seat" | awk '{print $1}')
do  
status=$(loginctl show-session -p Display -p Active $ids|awk '$1 ~ /Active=yes/ { print $ids}')

if [ "$status" = "Active=yes" ]; then
myuser=$(cat /run/systemd/sessions/"$ids"|grep "USER")
mydisplay=$(cat /run/systemd/sessions/"$ids"|grep "DISPLAY"|tail -1)
mydesktopmanager=$(cat /run/systemd/sessions/"$ids"|grep "SERVICE"|tail -1)

#myuser=$(loginctl list-sessions|awk -v idss="$ids" '$1 == idss {print $3}')
#mydisplay=$(loginctl show-session -p Display -p Active $ids|awk '$1 ~ /Display=/ { print $1}')

fi
done
# : karekteri siliniyor
myuser=$(echo "$myuser"| tr -d ':')
mydisplay=$(echo "$mydisplay"| tr -d ':')
mydesktopmanager=$(echo "$mydesktopmanager"| tr -d ':')

# = karekterinden sonraki sütun alınıyor
myuser=$(echo "$myuser"|awk -F '=' '{print $2}')
mydisplay=$(echo "$mydisplay"| awk -F '=' '{print $2}')
mydesktopmanager=$(echo "$mydesktopmanager"| awk -F '=' '{print $2}')

myid=$(id -u $myuser)
version=$(uname -a|awk '{print $3}'|cut -c1-1) 	#3.16=>3	4.19=>4	vb.
proje=$(uname -a|awk '{print $2}')		#etap 	pardus 	vb.

#echo "myenv|$myuser|$mydisplay|$myid|$proje$version|$mydesktopmanager" > /tmp/myenv
#/usr/bin/e-ag-client-env "eagclientenv" "myenv|$myuser|$mydisplay|$myid|$proje$version|$mydesktopmanager"
echo -n "myenv|$myuser|$mydisplay|$myid|$proje$version|$mydesktopmanager" >/dev/udp/127.0.0.1/6666
#echo -n "portStatus|mydisp|noLogin|0|0|0|0|myenv|noLogin|0|0|0|0|0|0|0|close" >/dev/udp/127.0.0.1/6666
#echo $mydisplay
pv=$(echo "$proje$version")


if [ "$pv" = "etap3" ]; then
  cp  /home/$myuser/.Xauthority /root
  chown root:root /root/.Xauthority
fi

if [ "$pv" = "etap4" ]; then
  if [ "$myuser" = "Debian-gdm" ]; then
    cp  /run/user/$myuser/.Xauthority /root
    chown root:root /root/.Xauthority
  else
    cp  /home/$myuser/.Xauthority /root
    chown root:root /root/.Xauthority
  fi
fi

if [ "$pv" = "pardus4" ]; then
  if [ "$myuser" = "lightdm" ]; then
    cp  /var/lib/lightdm/.Xauthority /root
    chown root:root /root/.Xauthority
  else
    cp  /home/$myuser/.Xauthority /root
    chown root:root /root/.Xauthority
  fi
fi
