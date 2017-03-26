if [[ -e vendor/slim ]]; then
    add_lunch_combo slim_hi6250-userdebug
elif [[ -e vendor/omni ]]; then
    add_lunch_combo omni_hi6250-userdebug
else
    add_lunch_combo lineage_hi6250-userdebug
fi
