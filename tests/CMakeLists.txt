add_executable(test-smoke smoke.c)
target_link_libraries(test-smoke brainfuck)
add_test(smoke test-smoke)

add_executable(test-ir ir.c)
target_link_libraries(test-ir brainfuck)
add_test(ir test-ir)
