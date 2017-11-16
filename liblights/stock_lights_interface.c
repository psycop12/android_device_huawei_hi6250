/*
 * Meticulus hi6250 liblights
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
#include <utils/Log.h>

void *handle = NULL;

int load_stock_lights(char *path, hw_module_t **pHmi) {
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

void close_stock_lights() {
    dlclose(handle);
    handle = NULL;
}

