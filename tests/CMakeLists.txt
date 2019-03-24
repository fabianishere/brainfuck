add_executable(test-smoke smoke.c)
target_link_libraries(test-smoke brainiac)
add_test(smoke test-smoke)

add_executable(test-ir ir.c)
target_link_libraries(test-ir brainiac)
add_test(ir test-ir)
