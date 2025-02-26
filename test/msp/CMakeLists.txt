add_executable(msp_test msp_test.c)

target_link_libraries(
  msp_test
  uart
  msp_serial
)

add_debug_advanced(msp_test)
