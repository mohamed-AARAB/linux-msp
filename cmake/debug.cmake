# look for the debugger
# file(GLOB gdb_bin ${CMAKE_FIND_ROOT_PATH}/bin/*gdb)
# message("gdb_bin : ${gdb_bin}")

function(add_debug target)
  get_target_property(target_bin_dir ${target} BINARY_DIR)
  get_target_property(target_source_dir ${target} BINARY_DIR)
  add_custom_target(
    debug_${target}
    COMMAND
      gdb-multiarch --command=${CMAKE_SOURCE_DIR}/gdbinit ${target_bin_dir}/${target}
  )
endfunction()

function(add_debug_advanced target)
  get_target_property(target_bin_dir ${target} BINARY_DIR)
  get_target_property(target_source_dir ${target} BINARY_DIR)
  add_custom_target(
    debug_${target}
    COMMAND
      gdb-multiarch  
      --command=${CMAKE_SOURCE_DIR}/gdbinit
      -eval-command \"tui enable\" 
      -eval-command \"tui focus cmd\" 
      -eval-command \"remote put ${target_bin_dir}/${target} /root/temp_prog\" 
      -eval-command \"set remote exec-file /root/temp_prog\" 
      -eval-command \"b main\" 
      -eval-command \"run\" 
      ${target_bin_dir}/${target}
  )
  add_custom_target(
    run_${target}
    COMMAND
      gdb-multiarch  
      --command=${CMAKE_SOURCE_DIR}/gdbinit
      -eval-command \"tui disable\" 
      -eval-command \"remote put ${target_bin_dir}/${target} /root/temp_prog\" 
      -eval-command \"set remote exec-file /root/temp_prog\" 
      -eval-command \"run\" 
      ${target_bin_dir}/${target}
  )
endfunction()

