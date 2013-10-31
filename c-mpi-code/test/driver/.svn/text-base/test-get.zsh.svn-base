test/driver/test-cmd-put.x   -d ${DRIVER_DIR} sample-key
test/driver/test-cmd-sleep.x -d ${DRIVER_DIR} -s 1
test/driver/test-cmd-get.x   -d ${DRIVER_DIR} sample-key
[[ $? == 0 ]] || bail "could not get: sample-key"
test/driver/test-cmd-quit.x  -d ${DRIVER_DIR}
