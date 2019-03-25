
## Dependencies ##
find_package(Lightning REQUIRED)

## Target ##
target_sources(brainiac PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/lightning.h
        ${CMAKE_CURRENT_LIST_DIR}/lightning.c)
target_link_libraries(brainiac PRIVATE lightning)
