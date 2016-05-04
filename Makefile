CC=/home/lubuntu/rpi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-gcc
APP_BINARY=kennismaking
REMOTE_SERVER=pi@robot7
REMOTE_PATH=/home/pi
REMOTE_PASS='raspberry'
ML=/home/lubuntu/minimod/Libs/
ML_ROB=/home/lubuntu/robotica/lib/
LOGFILE='log.csv'

CFLAGS += -g -DLINUX -Wall -I$(ML) -I$(ML_ROB) -I.
LFLAGS += -lpthread -L$(ML) -lrt

OBJECTS=main.o robotapp.o qrcode.o tagreader.o dijkstra.o rfcomms.o drive.o $(ML_ROB)timestep.o $(ML_ROB)TimeSupport.o $(ML)lin-rpi-serlib.o $(ML)lin-rs485client.o $(ML)crc.o $(ML)rf-cc1101.o $(ML)lego-motor.o $(ML)lego-sensor.o $(ML)lin-delay.o $(ML)lin-gpio.o $(ML)lin-lego-motor-log.o $(ML)pwr-liion1a.o $(ML)ansi.o $(ML)imu.o
#include shell scripts
SHELLSCRIPTS= QRDecoder.sh tagreader.sh mkRamdisk.sh cleanup.sh
DATAFILES= APDU.dat Graph.dat

all: kennismaking
Debug: kennismaking
kennismaking: $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $(APP_BINARY)

-include $(OBJECTS:.o=.d)
-include $(SHELLSCRIPTS:.sh)
-include $(DATAFILES:.dat)


%.o: %.c
	$(CC) -c $(CFLAGS) $*.c -o $*.o
	$(CC) -MM $(CFLAGS) $*.c > $*.d

cleanDebug: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(DATAFILES)
	rm -f $(OBJECTS:.o=.d)
	rm -f $(APP_BINARY)
install:
	chmod +x $(APP_BINARY)
	sshpass -p $(REMOTE_PASS) scp -o StrictHostKeyChecking=no  $(APP_BINARY) $(REMOTE_SERVER):$(REMOTE_PATH)
	sshpass -p $(REMOTE_PASS) scp -o StrictHostKeyChecking=no $(SHELLSCRIPTS) $(REMOTE_SERVER):$(REMOTE_PATH)
	sshpass -p $(REMOTE_PASS) scp -o StrictHostKeyChecking=no $(DATAFILES) $(REMOTE_SERVER):$(REMOTE_PATH)
rundebug:
	sshpass -p $(REMOTE_PASS) ssh -t $(REMOTE_SERVER)  "sudo gdbserver localhost:5000 $(REMOTE_PATH)/$(APP_BINARY)"
runnodebug:
	sshpass -p $(REMOTE_PASS) ssh -t $(REMOTE_SERVER) sudo $(REMOTE_PATH)/$(APP_BINARY)
plotlog:
	rm -f $(LOGFILE)
	sshpass -p $(REMOTE_PASS) scp -o StrictHostKeyChecking=no  $(REMOTE_SERVER):$(REMOTE_PATH)/$(LOGFILE) .
	octave --persist logplot.m
plotdelete:
	sshpass -p $(REMOTE_PASS) ssh -t $(REMOTE_SERVER) rm -f $(REMOTE_PATH)/$(LOGFILE)


