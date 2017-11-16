/*
 * Meticulus hi6250 libhuawei_init
 * Copyright (c) 2017 Jonathan Dennis [Meticulus]
 *                               theonejohnnyd@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Load variant specific props here */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/klog.h>
#include <sys/system_properties.h>
#include <unistd.h>
#include <stdio.h>
#define BASE "/sys/firmware/devicetree/base/"
#define PRODUCT_PATH BASE"hisi,product_name"
#define MODEM_ID_PATH BASE"hisi,modem_id"
#define PHONE_PROP_PATH "/vendor/phone.prop"
#define BOARDID_PRODUCT_PROP "ro.boardid.product"
#define PRODUCT_MODEL_PROP "ro.product.model"

static char *model = "hi6250";

extern "C" int __system_property_add(char *key,int ksize,char * value,int vsize);
extern "C" const prop_info * __system_property_find(const char * name);
extern "C" int __system_property_update(prop_info * pi, char* value,int vsize);
extern "C" int property_get(char *key, char* value, const char *dvalue);

static void set_property(char *key, char *value) {
    int error;
    error = __system_property_add(key,strlen(key),value,strlen(value));
    if(error) {
	klog_write(0, "huawei_init: Could not set %s to %s error %d\n",key,value,error);
    }
}

static void update_property(char *key, char *value) {
    int error;
    prop_info* pi = (prop_info*) __system_property_find(key);
    error = __system_property_update(pi,value,strlen(value));
    if(error) {
	klog_write(0, "huawei_init: Could not update %s to %s error %d\n",key,value,error);
    }
}

static char * read_string(char * path) {
    char var[255];
    sprintf(var,"%s","ERR");

    int fd = open(PRODUCT_PATH, O_RDONLY);
    if(fd < 0) {
	klog_write(0,"huawei_init: Could not open %s!\n", path);
    }
    if(read(fd,var,255) < 1) {
	klog_write(0,"huawei_init: Unable to read %s!\n", path);
    }
    return strdup(var);
}

static int read_int(char * path) {
     return atoi(read_string(path));
}

void vendor_load_default_properties() {
    model = read_string(PRODUCT_PATH);
    klog_write(0,"huawei_init: model is %s\n",model);

    /* bail if model is NULL */
    if(!model)
	return;

    /* All P9-Lite needs this */
    if(!strncmp(model,"VNS", 3)) 
	set_property(BOARDID_PRODUCT_PROP,"51316");
    /* All Honor 5c/Honor 7 lite needs this */
    if(!strncmp(model, "NEM", 3) || !strncmp(model, "NMO", 3))
	set_property(BOARDID_PRODUCT_PROP, "4871");

    /* These have renamed model */
    if(!strcmp(model, "NMO-L21"))
	model = "NEM-L51"; 

    /* if a match is not found the values in the build.prop will be used.
     * ro.boardid.product will not be set so the camera will not work.
     */
}
static void load_modem_props() {
    int fd = -1,retval = 0,on = 0;
    FILE * pf;
    char buff[255];
    char modemid[255];
    fd = open(MODEM_ID_PATH, O_RDONLY);
    if(fd < 0 ) {
	klog_write(0, "huawei_init: Couldn't get modem id?");
	return;
    }
    /* Meticulus:
     * The modem id is not characters, so convert to hex.
     */
    size_t size = read(fd, buff, 254);
    klog_write(0,"modemid = 0X%X%X%X%X%X\n", buff[0], buff[1],buff[2],buff[3],buff[4]);
    sprintf(modemid, "[0X%X%X%X%X%X]:\n", buff[0], buff[1],buff[2],buff[3],buff[4]);
    close(fd);
    pf = fopen(PHONE_PROP_PATH, "r");
    if(pf < 0) {
	klog_write(0, "huawei_init: Couldn't read phone.prop?");
	return;
    }
    char * linebuf;
    size = 0;
    /* Meticulus:
     * Loop through the prop file and look for the modem id.
     * Once found, set all props until you get empty line
     */
    while(getline(&linebuf, &size, pf) > -1) {
	if(!strcmp(linebuf, modemid)) {
	    klog_write(0, "huawei_init: Found! %s",linebuf);
	    on = 1;
	    continue;	
	}
	if(on && linebuf[0] == '\n') {
	    /* Meticulus: Empty line so we are done */
	    break;
	}
	else if(on) {
            /* Meticulus: Convert 'new line' to endof char * */
	    char *eol = strstr(linebuf, "\n");
	    eol[0] = '\0';
            /* Meticulus: Convert '=' to 'end of char * */
	    char *eq = strstr(linebuf, "=");
	    eq[0] = '\0';
	    char *value = (char *)eq + 1;
	    if(!strcmp("persist.radio.multisim.config", linebuf)) {
		value = "single";
	    }
	    set_property(linebuf, value);
	}
	linebuf = NULL;
	size = 0;
    }
    fclose(pf);
    if(!on) {
	klog_write(0, "huaei_init: modemid '%s' was not found in phone.prop",modemid);
    }
}
void vendor_load_system_properties() {
    load_modem_props();
}

