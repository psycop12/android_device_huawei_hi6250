/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (c) 2016 Jonathan Jason Dennis [Meticulus]
 *				theonejohnnyd@gmail.com
 *
 * Not a Contribution, Apache license notifications and license are retained
 * for attribution purposes only.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Load variant specific props here */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <cutils/klog.h>
#include <sys/system_properties.h>
#include <unistd.h>
static char PRODUCT_PATH[] = "/sys/firmware/devicetree/base/hisi,product_name";
static char MULTI_RILD_PROP[] = "ro.multi.rild";
static char CONFIG_MULTI_RILD_PROP[] = "ro.config.multirild";
/* 
 * Meticulus: even though this prop is mispelled, it might be that
 * this prop was originally misspelled by Huawei, and therefore
 * does work properly. 
 */
static char CONFIG_MULI_RILD_PROP[] = "ro.config.mulirild";
static char BOARDID_PRODUCT_PROP[] = "ro.boardid.product";
static char PRODUCT_MODEL_PROP[] = "ro.product.model";

static char TRUE[] = "true";
static char FALSE[] = "false";
static char AUTO[] = "auto";

static char model[PROP_VALUE_MAX] = "hi6250";

extern "C" int __system_property_add(char *key,int ksize,char * value,int vsize);
extern "C" const prop_info * __system_property_find(const char * name);
extern "C" int __system_property_update(prop_info * pi, char* value,int vsize);
extern "C" int property_get(char *key, char* value, const char *dvalue);

static void set_property(char *key, char *value) {
    int error;
    error = __system_property_add(key,strlen(key),value,strlen(value));
    if(error) {
	klog_write(0, "libhuawei_init: Could not set %s to %s error %d\n",key,value,error);
    }
}

static void update_property(char *key, char *value) {
    int error;
    prop_info* pi = (prop_info*) __system_property_find(key);
    error = __system_property_update(pi,value,strlen(value));
    if(error) {
	klog_write(0, "libhuawei_init: Could not update %s to %s error %d\n",key,value,error);
    }
}

static char * read_string(char * path) {
    char var[255];
    sprintf(var,"%s","ERR");

    int fd = open(PRODUCT_PATH, O_RDONLY);
    if(fd < 0) {
	klog_write(0,"Could not open %s!\n", path);
    }
    if(read(fd,var,255) < 1) {
	klog_write(0,"Unable to read %s!\n", path);
    }
    return strdup(var);
}

static int read_int(char * path) {
     return atoi(read_string(path));
}

void vendor_load_default_properties() {

    char boardid[PROP_VALUE_MAX];

    *model = * read_string(PRODUCT_PATH);

    klog_write(0,"libhuawei_init: model is %s\n",model);

    /* All P9-Lite needs this */
    if(!strncmp(model,"VNS", 3))
	sprintf(boardid, "%s", "51312");
    /* All Honor 5c/Honor 7 lite needs this */
    else if(!strncmp(model, "NEM", 3) || !strncmp(model, "NMO", 3))
	sprintf(boardid, "%s", "4871");

    set_property(BOARDID_PRODUCT_PROP, boardid);

    /* These have renamed model */
    if(!strcmp(model, "NMO-L21")) {
	sprintf(model,"%s","NEM-L51");
    }

    /* if a match is not found the values in the build.prop will be used.
     * ro.boardid.product will not be set so the camera will not work.
     */
}

void set_if_not_set_to(char * name, char * if_not_set_to, char * new_value) {
    char value[PROP_VALUE_MAX];
    if(!property_get(name, value, if_not_set_to))
	klog_write(0, "libhuawei_init: Could not get property %s\n", name);
    else if(!strcmp(value,if_not_set_to))
	    set_property(name , new_value);

}

void set_model() {
    char lmodel[PROP_VALUE_MAX];

    if(!property_get(PRODUCT_MODEL_PROP,lmodel,"hi6250"))
	klog_write(0, "libhuawei_init: Could not get property %s\n",PRODUCT_MODEL_PROP);
    else if(!strcmp(lmodel,"hi6250"))
	update_property(PRODUCT_MODEL_PROP,model);

}
void vendor_load_system_properties() {

    set_model();

    if(!strcmp(model, "VNS-L31") ||
	    !strcmp(model, "VNS-L22") ||
	    !strcmp(model, "NEM-L21")) {
	set_if_not_set_to(MULTI_RILD_PROP, AUTO, TRUE);
	set_if_not_set_to(CONFIG_MULTI_RILD_PROP, AUTO, TRUE);
	set_if_not_set_to(CONFIG_MULI_RILD_PROP, AUTO, TRUE);
    } else {
	set_if_not_set_to(MULTI_RILD_PROP, AUTO, TRUE);
	set_if_not_set_to(CONFIG_MULTI_RILD_PROP, AUTO, TRUE);
	set_if_not_set_to(CONFIG_MULI_RILD_PROP, AUTO, TRUE);
    }
}

