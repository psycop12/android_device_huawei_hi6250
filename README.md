# CyanogenMod 13 for the Huawei P9 Lite (hi6250)

### Kernel
This device tree is intended to be used with this kernel repo:

See: https://github.com/Meticulus/android_kernel_huawei_hi6250

branch: master
### Vendor
This device tree is intented to be used with this vendor repo

https://github.com/Meticulus/android_vendor_huawei_hi6250

branch: master
# How To Build

### Step 1: Setting up the Build Environment.

You'll need Linux to be able to build Cyanogenmod. You have three choices here; you can:

1. Install Linux natively on your computer.
2. Dual boot Linux and Windows.
3. Use virtual machine software ( virtual box, vmware ) to run linux.

NOTE: I recommend you use Ubuntu 14.04 LTS to build. That's what I use.

Now read this: http://source.android.com/source/initializing.html

NOTE: When I say "read", I mean read and comprehend.

In addition to Android's build prerequisites CyanogenMod requires "maven"
```bash
sudo apt-get install maven2
```

NOTE: Read through the topics here as well: http://wiki.cyanogenmod.org/w/Development
### Step 2: Downloading the Source.

NOTE: Some say that it is better to download the ROM source and put in your local manifest later. I don't know if that's best but that's what we are going to do.

BEFORE YOU BEGIN: You are about to start downloading 5 - 15 Gigs of data. That could take a very long time, so plan accordingly. I like to start just before I go to sleep and let it go overnight! If you have to work, perhaps start it before work and let it go through out the day.

Execute the following commands in a linux terminal:
```bash
mkdir /home/$USER/cm
cd /home/$USER/cm
repo init -u git://github.com/CyanogenMod/android.git -b cm-13.0
repo sync
```
WARNING: There may be times, towards the end when it seem like, the download is stuck and not making any progress because there are no updates on the screen. BE PATIENT!, open a program that will show how much bandwidth you are using to be sure!

### Step 3: Set up local manifest.

The local manifest is different for every device. It contains those repos that are device specific, where as the ROM code you just "repo sync'd" is code that is general to any device.

NOTE: Meticulus Development also builds some extra features and functions into its builds and we include the source for those builds in this tree as well. If you would like to include those "extras", then when asked to issue the "./patch.sh" command, include the keyword "meticulus" after it e.g. "./patch.sh meticulus". We respectfully ask that you do not publish or distribute these builds.

Execute the following commands in a linux terminal:
```bash
mkdir /home/$USER/cm/.repo/local_manifests
gedit /home/$USER/cm/.repo/local_manifests/hi6250.xml
```
Copy the following into hi6250.xml, save and close.
```bash
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
  <project name="Meticulus/android_kernel_huawei_hi6250" path="kernel/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_device_huawei_hi6250" path="device/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_vendor_huawei_hi6250" path="vendor/huawei/hi6250" remote="github" revision="master"/>
  <remove-project name="platform/packages/apps/Nfc"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_Nfc" path="vendor/nxp-nfc/opensource/Nfc" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_libnfc-nci" path="vendor/nxp-nfc/opensource/libnfc-nci" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_frameworks" path="vendor/nxp-nfc/opensource/frameworks" remote="github" revision="cm-13.0"/>
</manifest>
```

Alternatively, if you would like to include Meticulus Development's "extras" then copy the following into hi6250.xml, save and close.
```bash
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
  <project name="Meticulus/android_kernel_huawei_hi6250" path="kernel/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_device_huawei_hi6250" path="device/huawei/hi6250" remote="github" revision="master"/>
  <project name="Meticulus/android_vendor_huawei_hi6250" path="vendor/huawei/hi6250" remote="github" revision="master"/>
  <remove-project name="platform/packages/apps/Nfc"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_Nfc" path="vendor/nxp-nfc/opensource/Nfc" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_libnfc-nci" path="vendor/nxp-nfc/opensource/libnfc-nci" remote="github" revision="cm-13.0"/>
  <project name="CyanogenMod/android_vendor_nxp-nfc_opensource_frameworks" path="vendor/nxp-nfc/opensource/frameworks" remote="github" revision="cm-13.0"/>
  <project name="Meticulus/android_packages_CodinalteParts" path="packages/apps/CodinalteParts" remote="github" revision="hi6250"/>
  <project name="omnirom/android_packages_PerformanceControl" path="packages/apps/PerformanceControl" remote="github" revision="android-4.4"/>
</manifest>
```

Execute the following commands in a linux terminal:
```bash
cd /home/$USER/cm
repo sync
```

NOTE: Yes we are syncing again and No, it shouldn't take quite as long. Every time you repo sync just new data is downloaded. So we are downloading the 4 repo's we just put in and any updates that may have occured to the repo's we already have.

### Step 4: Building

Now you will want to apply the repo patches. These patches modify code in the ROM to work with this device.
Execute the following commands in a linux terminal:
```bash
cd /home/$USER/cm/device/huawei/hi6250/patches
./patch.sh
```
NOTE: If you are going to be offline while you are building, you will need download some prebuilts first.
```bash
cd /home/$USER/cm
make fetchprebuilts
```
NOTE: Now you have everything that you need to build CyanogenMod for your Huawei P9 Lite. Build times depend on you PC performance specifications. In the following terminal command "-j8" represents the number of concurrent tasks to execute. For low specs machines (such as mine) lowering the value to "-j3" may help speed things up. For high spec'd machines raising the value may speed things up.

NOTE: It may take anywhere from 5 hours to 15 hours depending on system specs for a complete build.
Execute the following commands in a linux terminal:
```bash
cd /home/$USER/cm
. build/envsetup.sh
lunch cm_hi6250-userdebug
make -j8 bacon
```

# Build TWRP ( optional )

NOTE: Currently TWRP can only be built after a successful build of CyanogenMod 13.

### Step 1: Setup build env for TWRP
Clone OMNI's recovery (TWRP) into the build system.
```bash
cd /home/$USER/cm
git clone https://github.com/omnirom/android_bootable_recovery.git -b android-6.0 bootable/recovery-twrp
```

Open the BoardConfig.mk file and remove the comment "#" before the line "RECOVERY_VARIANT := twrp".
Find BOARD_KERNEL_CMDLINE and change androidboot.selinux=disabled to androidboot.selinux=permissive.
Save BoardConfig.mk

Re-run patch.sh to patch bootable/recovery-twrp
```bash
cd /home/$USER/cm/device/huawei/hi6250/patches
./patch.sh
```
### Step 2: Build TWRP

```bash
cd /home/$USER/cm
. build/envsetup.sh
lunch cm_hi6250-userdebug
make -j8 recoveryimage
```

NOTE: remember to re-add the "#" before "RECOVERY_VARIANT := twrp" and change androidboot.selinux back to 'disabled' in the BoardConfig.mk before doing a clean build or it will fail or not boot!
### Please let me know if there are mistakes,typos, mis/outdated - information in these instructions by creating an "issue".
