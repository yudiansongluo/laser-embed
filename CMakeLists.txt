# CMake minimum version
cmake_minimum_required(VERSION 3.11)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
# Subtitude the linker file to your Chip

# https://stackoverflow.com/questions/53633705/cmake-the-c-compiler-is-not-able-to-compile-a-simple-test-program
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# CMake can't identify if the OS is WIN32 or UNIX in MSYS Makefiles
if(UNIX)
  set(CROSS_TARGET_TRIPLET "arm-none-eabi-")
  set(CMAKE_C_COMPILER ${CROSS_TARGET_TRIPLET}gcc)
  set(CMAKE_ASM_COMPILER ${CROSS_TARGET_TRIPLET}gcc)
endif(UNIX)

# if you're in Windows please use MSYS Makefiles Generator Maybe MinGW Makefiles
# Generator is also works cmake . -G "MSYS Makefiles"
if(WIN32)
  # Why don't you open it in Visual Studio? 
  # Force Windows to use MSYS Makefiles Generator
  # set(CMAKE_GENERATOR
  #     "MSYS Makefiles"
  #     CACHE INTERNAL "" FORCE)
  set(tools
      "C:/Program Files (x86)/GNU Tools ARM Embedded/5.4 2016q3")
  set(tools_bin ${tools}/bin/)
  set(tools_include ${tools}/arm-none-eabi/include/)
  set(gcc "gcc.exe")
  set(CROSS_TARGET_TRIPLET "arm-none-eabi-")
  set(CMAKE_C_COMPILER ${tools_bin}${CROSS_TARGET_TRIPLET}${gcc})
  set(CMAKE_ASM_COMPILER ${tools_bin}${CROSS_TARGET_TRIPLET}${gcc})
endif(WIN32)

# Project Infomation
project(laser-embed C ASM)
set(mcu "cortex-m3")
set(linker "stm32f103c8tx.ld")

# CC AR LD AS

enable_language(ASM)
enable_language(C)

# Reset output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# STDLIB
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# We do not recommend using GLOB to collect a list of source files from your
# source tree Code from
# https://stackoverflow.com/questions/31004421/does-cmake-support-include-directories-recursively
# https://stackoverflow.com/questions/17653738/recursive-cmake-search-for-header-and-source-files
macro(HEADER_DIRECTORIES return_list)
  file(GLOB_RECURSE new_list *.h)
  set(dir_list "")
  foreach(file_path ${new_list})
    get_filename_component(dir_path ${file_path} PATH)
    set(dir_list ${dir_list} ${dir_path})
  endforeach()
  list(REMOVE_DUPLICATES dir_list)
  set(${return_list} ${dir_list})
endmacro()

header_directories(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

# Set include path use macro include_directories(${header_dir_list})

include_directories(
  User
  Libraries/CMSIS
  Libraries/STM32F10x_StdPeriph_Driver/inc
  APP/led
  Public
  APP/beep
  APP/smg
  APP/key
  APP/exti
  APP/time
  APP/pwm
  APP/iwdg
  APP/input
  APP/touch_key
  APP/wkup
  APP/adc
  APP/adc_temp
  APP/dac
  APP/pwm_dac
  APP/dma
  APP/rtc
  APP/24Cxx
  APP/iic
  APP/ds18b20
  APP/hwjs
  APP/rs485
  APP/can
  APP/tftlcd
  APP/hwjs
  APP/hexify
  APP/nrf24l01
  APP/spi
  APP/input
  APP/gps
  APP/function
  APP/module
        APP/voice
)

# CFLAGS
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
# "--specs=nosys.specs" "--specs=nano.specs" warning: cannot find entry symbol
# Reset_Handler; defaulting to 08000000
set(CMAKE_C_FLAGS
    "--specs=nosys.specs \
    -g \
    -Wextra \
    -Wshadow \
    -Wimplicit-function-declaration \
    -Wredundant-decls \
    -Wmissing-prototypes \
    -fno-common -ffunction-sections \
    -fdata-sections \
    -MD \
    -Wall \
    -Wundef \
    -mthumb \
    -mcpu=${mcu}"
    CACHE INTERNAL "c compiler flags")
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} \
-D STM32F10X_HD  \
-D USE_STDPERIPH_DRIVER")

# CXXFLAGS
set(CMAKE_CXX_FLAGS
    "-Wextra \
    -Wshadow \
    -Wredundant-decls  \
    -Weffc++ \
    -fno-common \
    -ffunction-sections \
    -fdata-sections \
    -MD \
    -Wall \
    -Wundef \
    -mthumb \
    -mcpu=${mcu}"
    CACHE INTERNAL "cxx compiler flags")

# ASMFLAGS
set(CMAKE_ASM_FLAGS
    "-g \
    -mthumb \
    -mcpu=${mcu} "
    CACHE INTERNAL "asm compiler flags")

# LDFLAGS
set(CMAKE_EXE_LINKER_FLAGS
    "-g \
    -Wl,--gc-sections \
    -Wl,-Map=${EXECUTABLE_OUTPUT_PATH}/output.map \
    -mthumb -mcpu=${mcu}"
    CACHE INTERNAL "executable linker flags")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
    -T ${CMAKE_SOURCE_DIR}/${linker}")

# YOU MUST USE file insted of set function inorder to include a file
file(GLOB_RECURSE START_FILE Libraries/CMSIS/startup_stm32f10x_hd.s
)# use file to include a fucking file
file(GLOB_RECURSE USER_SRC User/*.c)
file(GLOB_RECURSE STD_DRIVER_SRC Libraries/STM32F10x_StdPeriph_Driver/*.c)
# Generate the target
add_executable(
  ${CMAKE_PROJECT_NAME}.elf
  ${START_FILE}
  ${USER_SRC}
  ${STD_DRIVER_SRC} # Maybe you should select them manually
  Libraries/CMSIS/system_stm32f10x.c
  APP/led/led.c
  APP/tftlcd/tftlcd.c
  APP/hwjs/hwjs.c
  APP/nrf24l01/nrf24l01.c
  APP/spi/spi.c
  APP/gps/gps.c
  APP/module/module.c
  APP/function/function.c
  APP/time/time.c
  Public/system.c
  Public/SysTick.c
  Public/usart.c
  APP/voice/voice.c)


# Link the library to the target set(LIBS "-lc -lm -lnosys" )
# target_link_libraries (${CMAKE_PROJECT_NAME}.elf ${LIBS})
target_link_libraries(${CMAKE_PROJECT_NAME}.elf)

# Generate the binary file
add_custom_target(
  ${CMAKE_PROJECT_NAME}.bin ALL
  arm-none-eabi-objcopy -Obinary
  "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf"
  "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.bin"
  DEPENDS ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf)

# Generate the hex file
add_custom_target(
  ${CMAKE_PROJECT_NAME}.hex ALL
  arm-none-eabi-objcopy -Oihex
  "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf"
  "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.hex"
  DEPENDS ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf)

# Echo the size Infomation
add_custom_target(
  size ALL
  arm-none-eabi-size "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf"
  DEPENDS ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.elf)

# Make flash to the board by st-link
add_custom_target(
  flash COMMAND st-flash write
                ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_PROJECT_NAME}.bin 0x8000000)

# Make clean-all
add_custom_target(clean-all COMMAND rm -rf ${CMAKE_BINARY_DIR}/*)
