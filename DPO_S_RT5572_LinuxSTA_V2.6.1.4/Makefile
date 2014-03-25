all:
	make -f Makefile.inc
	mkdir -p ./LIB/${PLATFORM}/
	cp -f ./os/linux/rt5572sta.ko ./LIB/${PLATFORM}
	cp ./RT2870STA.dat ./LIB/${PLATFORM}
	${CROSS_COMPILE}strip -g ./LIB/${PLATFORM}/*.ko
	chmod +x ./LIB/${PLATFORM}/*
	cp ./WIFI* ./LIB/${PLATFORM}
	echo "copy modules to CHERRY_DIR"
	cp -vf ./LIB/${PLATFORM}/* ${AP_CNC_DIR}/AP_CherrySS/Driver/${TARGET}/RALINK_WIFI


clean:
	make -f Makefile.inc clean
