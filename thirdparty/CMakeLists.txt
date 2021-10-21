include(FetchContent)

## Add malloc_count
FetchContent_Declare(
  malloc_count
  GIT_REPOSITORY https://github.com/bingmann/malloc_count
  )
  
FetchContent_GetProperties(malloc_count)
if(NOT malloc_count_POPULATED)
  FetchContent_Populate(malloc_count)

  add_library(malloc_count OBJECT ${malloc_count_SOURCE_DIR}/malloc_count.c ${malloc_count_SOURCE_DIR}/malloc_count.h)
  target_link_libraries(malloc_count dl)
  target_include_directories(malloc_count PUBLIC "${malloc_count_SOURCE_DIR}")

  add_library(memprofile OBJECT ${malloc_count_SOURCE_DIR}/memprofile.h)
  target_include_directories(memprofile PUBLIC "${malloc_count_SOURCE_DIR}")
endif()