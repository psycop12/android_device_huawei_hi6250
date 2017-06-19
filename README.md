# LineageOS 14.1 for the Huawei P9 Lite (hi6250)

### Kernel
This device tree is intended to be used with this kernel repo:

See: https://github.com/Meticulus/android_kernel_huawei_hi6250

branch: master
### Vendor
This device tree is intented to be used with this vendor repo

https://github.com/Meticulus/android_vendor_huawei_hi6250

branch: master
# How To Build
This "How to" is scoped for new builders with moderate PC skills.

### Step 1: Setting up the Build Environment.

You'll need Linux to be able to build LineageOS. You have three choices here; you can:

1. Install Linux natively on your computer.
2. Dual boot Linux and Windows.
3. Use virtual machine software ( virtual box, vmware ) to run linux.

NOTE: I recommend you use Ubuntu 14.04 LTS to build. That's what I use.

NOTE: DISK SPACE - You'll need about 110GB of free space for the build; swap space ( depending on PC specs ), and some head room. I'd say 130GB at a minimum but more is better if you can spare it.

## Now read this: http://source.android.com/source/initializing.html

Update OpenJava for Ubuntu 14.04: The OpenJava files listed on the above page are out-dated. Refer to this post on XDA: 
https://forum.xda-developers.com/oneplus-3/how-to/guide-complete-guide-building-t3477198/post70646310#post70646310

Terminal:
```bash
sudo add-apt-repository ppa:openjdk-r/ppa
sudo apt-get update
sudo apt-get install openjdk-8-jdk
```
LineageOS 14.1 uses ImageMagick in building the bootanimation.zip

Terminal:
```bash
sudo apt-get install imagemagick
```

### Step 2: Downloading the Source.

NOTE: Some say that it is better to download the ROM source and put in your local manifest later. I don't know if that's best but that's what we are going to do.

BEFORE YOU BEGIN: You are about to start downloading 15 - 20 Gigs of data. That could take a very long time, so plan accordingly. I like to start just before I go to sleep and let it go overnight! If you have to work, perhaps start it before work and let it go through out the day.

Execute the following commands in a linux terminal:

Terminal:
```bash
mkdir /home/$USER/los
cd /home/$USER/los
repo init -u git://github.com/LineageOS/android.git -b cm-14.1
repo sync
```
WARNING: There may be times, towards the end when it seem like, the download is stuck and not making any progress because there are no updates on the screen. BE PATIENT!, open a program that will show how much bandwidth you are using to be sure!

### Step 3: Set up local manifest.

The local manifest is different for every device. It contains those repos that are device specific, where as the ROM code you just "repo sync'd" is code that is general to any device.

NOTE: Meticulus Development also builds some extra features and functions into its builds and we include the source for those builds in this tree as well.

Execute the following commands:

Terminal:
```bash
mkdir /home/$USER/los/.repo/local_manifests
gedit /home/$USER/los/.repo/local_manifests/hi6250.xml
```
Copy the following into hi6250.xml, save and close.
```bash
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
  <project name="Meticulus/android_kernel_huawei_hi6250" path="kernel/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_device_huawei_hi6250" path="device/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_vendor_huawei_hi6250" path="vendor/huawei/hi6250" remote="github" revision="master"/>
  <project name="LineageOS/android_vendor_nxp-nfc_opensource_Nfc" path="vendor/nxp-nfc/opensource/Nfc" remote="github" revision="cm-14.1"/>
  <project name="LineageOS/android_vendor_nxp-nfc_opensource_libnfc-nci" path="vendor/nxp-nfc/opensource/libnfc-nci" remote="github" revision="cm-14.1"/>
  <project name="LineageOS/android_vendor_nxp-nfc_opensource_frameworks" path="vendor/nxp-nfc/opensource/frameworks" remote="github" revision="cm-14.1"/>
</manifest>
```

Alternatively, if you would like to include Meticulus Development's "extras" then copy the following into hi6250.xml, save and close.
```bash
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
  <project name="Meticulus/android_kernel_huawei_hi6250" path="kernel/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_device_huawei_hi6250" path="device/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_vendor_huawei_hi6250" path="vendor/huawei/hi6250" remote="github" revision="master"/>
  <project name="LineageOS/android_vendor_nxp-nfc_opensource_Nfc" path="vendor/nxp-nfc/opensource/Nfc" remote="github" revision="cm-14.1"/>
  <project name="LineageOS/android_vendor_nxp-nfc_opensource_libnfc-nci" path="vendor/nxp-nfc/opensource/libnfc-nci" remote="github" revision="cm-14.1"/>
  <project name="LineageOS/android_vendor_nxp-nfc_opensource_frameworks" path="vendor/nxp-nfc/opensource/frameworks" remote="github" revision="cm-14.1"/>
  <project name="Meticulus/android_packages_apps_CodinalteParts" path="packages/apps/CodinalteParts" remote="github" revision="hi6250"/>
</manifest>
```

Execute the following commands:

Terminal:
```bash
cd /home/$USER/los
repo sync
```

NOTE: Yes we are syncing again and No, it shouldn't take quite as long. Every time you repo sync just new data is downloaded. So we are downloading the 6 or 7 repo's we just put in and any updates that may have occured to the repo's we already have.

### Step 4: Building

Are you Meticulus? (optional)
If you want to include Meticulus Development extras

Terminal:
```bash
export I_AM_METICULUS=true
```

Turn off Ninja (optional)
Ninja is supposed to help make incremental updates more efficient but you aren't doing an incremental update; are you?

Terminal:
```bash
export USE_NINJA=false
```

Make sure there is enough memory for Jack(recommended).
This may not be neccessary depending on your PC's specs. On my previous laptop ( only 4Gig of mem ), I could not build without this and I keep it just in case.

Terminal:
```bash
export ANDROID_JACK_VM_ARGS="-Xmx4g -Dfile.encoding=UTF-8 -XX:+TieredCompilation"
```

Bake su, "root ", inside bacon (optional)

Terminal:
```bash
export WITH_SU=true
```

Turn on autopatch (recommended)
autopatch will apply device specific patches everytime you lunch.

Terminal:
```bash
export AUTOPATCH=true
```

If you did not turn on autopatch, you will want to apply the repo patches manually. These patches modify code in the ROM to work with this device.
Execute the following commands:

Terminal:
```bash
cd /home/$USER/los/device/huawei/hi6250/patches && ./patch.sh
```

NOTE: Now you have everything that you need to build LineageOS 14.1 for your hi6250 device. Build times depend on your PC's performance specifications. In the following terminal command "-j8" represents the number of concurrent tasks to execute. For low specs machines (such as mine) lowering the value to "-j3" may help speed things up. For high spec'd machines raising the value may speed things up.

NOTE: It may take anywhere from 1 hours to 15 hours depending on system specs for a complete build.
Execute the following commands:

Terminal:
```bash
cd /home/$USER/los
. build/envsetup.sh
lunch lineage_hi6250-userdebug
make -j8 bacon
```

NOTE: These trees can also build Resurection Remix (nougat), SlimRoms (nougat), and OmniRom (nougat) without modification but with logical changes to these instructions.
1. When running "repo init ...", you will obviously want to download the code for the respective ROM.
2. When building SlimRoms (nougat) or OmniRom (nougat) and running "lunch ...", you want slim_hi6250-userdebug or omni_hi6260-userdebug respectively.


### Please let me know if there are mistakes,typos, mis/outdated - information in these instructions by creating an "issue".
