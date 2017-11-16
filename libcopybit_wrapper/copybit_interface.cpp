/*
* Meticulus hi6250 libcopybit_wrapper
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
#include <hardware/hardware.h>

#include <cutils/properties.h>

#include <dlfcn.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include "copybit.h"
#include <utils/Log.h>

void *handle = NULL;

int load_real_copybit(char *path, hw_module_t **pHmi) {
    int status = -EINVAL;
    struct hw_module_t *hmi = NULL;


    handle = dlopen(path, RTLD_NOW);
    if (handle == NULL) {
        char const *err_str = dlerror();
	printf("load: module=%s\n%s\n", path, err_str?err_str:"unknown");
        status = -EINVAL;
	return status;
    } else {
	printf("load: module=%s %s\n", path,"Success!");
    }
    
    /* Get the address of the struct hal_module_info. */
    const char *sym = HAL_MODULE_INFO_SYM_AS_STR;
    hmi = (struct hw_module_t *)dlsym(handle, sym);
    if (hmi == NULL) {
        ALOGE("load: couldn't find symbol %s", sym);
        status = -EINVAL;
	dlclose(handle);
	return status;
    }
    hmi->dso = handle;
    *pHmi = hmi;
    ALOGI("module: tag=%d api_version=%d id=%s name=%s author=%s", hmi->tag, hmi->module_api_version,hmi->id, hmi->name, hmi->author);
    status = 0;
    return status;
}

int open_real_copybit(hw_module_t *module,hw_device_t **real_device) {
    return module->methods->open(module,"copybit0", real_device); 
}
int close_real_copybit(hw_device_t *real_device) {
    return ((copybit_device_t *)real_device)->common.close(&((copybit_device_t *)real_device)->common);
}

void close_fake_copybit() {
    dlclose(handle);
    handle = NULL;
}

int real_set_parameter(hw_device_t *real_device,int name, int value) {
    return ((copybit_device_t *)real_device)->set_parameter((copybit_device_t *)real_device,name,value);
}
int real_get(hw_device_t *real_device,int name) {
    return ((copybit_device_t *)real_device)->get((copybit_device_t *)real_device,name);
}

int real_blit(struct hw_device_t *real_device,
 		struct copybit_image_t const *dst,
                struct copybit_image_t const *src,
                struct copybit_region_t const *region) {
    return ((copybit_device_t *)real_device)->blit((copybit_device_t *)real_device,dst,src,region);
}

int real_set_sync(hw_device_t *real_device,int acquireFenceFd) {
    return ((copybit_device_t *)real_device)->set_sync((copybit_device_t *)real_device,acquireFenceFd);
}

int real_stretch(struct hw_device_t *real_device,
                   struct copybit_image_t const *dst,
                   struct copybit_image_t const *src,
                   struct copybit_rect_t const *dst_rect,
                   struct copybit_rect_t const *src_rect,
                   struct copybit_region_t const *region) {
    return ((copybit_device_t *)real_device)->stretch((copybit_device_t *)real_device,dst,src,dst_rect,src_rect,region);
}

int real_fill_color(struct hw_device_t *real_device,
                      struct copybit_image_t const *dst,
                      struct copybit_rect_t const *rect,
                      uint32_t color) {
    return ((copybit_device_t *)real_device)->fill_color((copybit_device_t *)real_device,dst,rect,color);
}
  
int real_finish(struct hw_device_t *real_device) {
    return ((copybit_device_t *)real_device)->finish((copybit_device_t *)real_device);
}
int real_flush_get_fence(struct hw_device_t *real_device, int *fd) {
    return ((copybit_device_t *)real_device)->flush_get_fence((copybit_device_t *)real_device, fd);
}

int real_clear(struct hw_device_t *real_device, struct copybit_image_t const *buf,
               struct copybit_rect_t *rect) {
    return ((copybit_device_t *)real_device)->clear((copybit_device_t *)real_device, buf, rect);
}

