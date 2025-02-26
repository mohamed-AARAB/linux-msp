# the name of the target operating system
set(CMAKE_SYSTEM_NAME Linux)

set( CMAKE_SYSTEM_PROCESSOR "armv7l" )

set(RK_TC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../luckfox-pico/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf)

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   ${RK_TC_PATH}/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${RK_TC_PATH}/bin/arm-rockchip830-linux-uclibcgnueabihf-g++)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH  ${RK_TC_PATH})

# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
