grep -rwh "STA_DRIVER_VERSION" . | grep -s "define" | sed "s/\"//" | sed "s/\"//" | awk 'NR == 1 {print $3}' | sed "s/\(^\)/\WIFI:/" > WIFI_INT_VERSION_INFO
cp WIFI_INT_VERSION_INFO WIFI_EXT_VERSION_INFO
chmod uga+r WIFI_*
