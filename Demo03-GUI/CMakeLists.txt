set(TARGET_NAME Demo03-GUI)
add_executable(${TARGET_NAME} DemoGUI.cpp)

BuildProperties(${TARGET_NAME})
target_link_libraries(${TARGET_NAME} ionGUI)
target_link_libraries(${TARGET_NAME} ionGraphicsGL)
target_link_libraries(${TARGET_NAME} ionGraphics)
target_link_libraries(${TARGET_NAME} ionWindow)
target_link_libraries(${TARGET_NAME} ionCore)
ExecutableProperties(${TARGET_NAME})
