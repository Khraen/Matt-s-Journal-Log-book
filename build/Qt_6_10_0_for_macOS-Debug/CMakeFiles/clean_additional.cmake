# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Diary_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Diary_autogen.dir/ParseCache.txt"
  "Diary_autogen"
  )
endif()
