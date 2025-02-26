
add_library(msp_serial msp_serial.c ../util/streambuf.c ../util/crc.c)

target_include_directories(
  msp_serial
  PUBLIC
  .
  ../util
)

target_link_libraries(
  msp_serial
  uart
)
