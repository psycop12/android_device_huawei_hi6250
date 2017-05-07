#include <hardware/hardware.h>

#include <cutils/properties.h>

#include <dlfcn.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include <hardware/hwcomposer.h>
#include <utils/Log.h>

void *handle = NULL;

int load_real_hwc(char *path, hw_module_t **pHmi) {
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
    printf("module: tag=%d api_version=%d id=%s name=%s author=%s\n", hmi->tag, hmi->module_api_version,hmi->id, hmi->name, hmi->author);
    status = 0;
    return status;
}

int open_real_hwc(hw_module_t *module,hw_device_t **real_device) {
    return module->methods->open(module,HWC_HARDWARE_COMPOSER, real_device); 
}
