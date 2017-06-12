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

#define VOLUMEINPUTPATH "/dev/input/event0"

int main(int argc, char *argv[]) {
    int fd = -1,retval = -2;
    struct input_event ev;
    int size = sizeof(struct input_event);
    fd = open(VOLUMEINPUTPATH, O_RDONLY);
    if (fd) {
restart:
	read(fd, &ev, size);
	if(ev.type ==  1) {
	    switch (ev.code) {
		case 114:
		    printf("down\n");
		    retval = -1;
		    break;
		case 115:
		    printf("up\n");
		    retval = 0;
		    break;
		default:
		    goto restart;
	    }
	}
	close(fd);
    } else {
		printf("Could not open %s\n", VOLUMEINPUTPATH);
    }
    return retval;
}


