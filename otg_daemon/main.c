/*
 *   Copyright (C) 2017  Jonathan Jason Dennis [Meticulus]
 *                                       theonejohnnyd@gmail.com
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as
 *   published by the Free Software Foundation, either version 3 of the
 *   License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.#include <linux/input.h>
 */

#include <linux/input.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cutils/klog.h>

#define USB_SWITCH_PATH "/sys/class/usb_switch/switch_ctrl/manual_ctrl/switchctrl"
#define PLUGUSB_PATH "/sys/devices/platform/ff100000.hisi_usb/plugusb"
#define CMDLINE_PATH "/proc/cmdline"
#define BLUE_PATH "/sys/class/leds/blue/brightness"

void write_file(char *path, char *value) {
    int fd = -1;
    size_t size = 0;
    fd = open(path, O_WRONLY);
    if(!fd) {
	klog_write(0, "MetiOTG: Could not open %s\n", path);
	return;
    }
    size = write(fd, value, strlen(value));
    close(fd);
    if(size != strlen(value))
	klog_write(0, "MetiOTG: Error writing to %s", path);
}

int check_recovery() {
    int fd = -1, ret = 0;
    char buff[255];
    fd = open(CMDLINE_PATH, O_RDONLY);
    if(!fd) {
	klog_write(0, "MetiOTG: Could not open %s\n", CMDLINE_PATH);
	return 0;
    }
    read(fd, buff, 254);
    if(strstr(buff, "enter_recovery=1"))
	ret = 1;

    return ret;
}

int main(int argc, char *argv[]) {
    int fd = -1,retval = -2,switchval = 0, hoston = 0, in_recovery = 0;
    char buff[255];
    klog_write(0, "MetiOTG: Startin' up.");
    in_recovery = check_recovery();
    fd = open(USB_SWITCH_PATH, O_RDONLY);
    if(!fd) {
	klog_write(0, "MetiOTG: Could not open %s; exiting!\n", USB_SWITCH_PATH);
	exit(-1);
    }
    while(1) {
	pread(fd, buff, 254, 0);
	switchval = atoi(buff);
	if(!hoston && switchval == 8) {
	    klog_write(0, "MetiOTG: OTG Cable plug detected. hoston!");
	    write_file(PLUGUSB_PATH, "hoston");
	    hoston = 1;
	    if(in_recovery)
		write_file(BLUE_PATH, "255");
	} else if (hoston && switchval != 8) {
	    klog_write(0, "MetiOTG: OTG Cable unplug detected. hostoff!");
	    write_file(PLUGUSB_PATH, "hostoff");
	    hoston = 0;
	    if(in_recovery)
		write_file(BLUE_PATH, "0");
	}
	
	sleep(1);
    }
}


