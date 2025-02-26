add_executable(uart_test uart_test.c)

target_link_libraries(
  uart_test
  uart
)
