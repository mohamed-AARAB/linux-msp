add_subdirectory(uart)
add_subdirectory(msp)

add_executable(inav_msp main.c ./util/bitarray.c)

target_include_directories(
  inav_msp
  PRIVATE
  .
  ./util
)

target_link_libraries(
  inav_msp
  uart
  msp_serial
)

add_debug_advanced(inav_msp)
