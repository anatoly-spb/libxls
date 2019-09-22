cmake_minimum_required(VERSION 3.12)

project(xls LANGUAGES C VERSION 1.5.1)

###################################################################

configure_file(
	config.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	@ONLY
)
add_library(xls STATIC
	config.h.in
	include/xls.h
	include/libxls/brdb.h
	include/libxls/endian.h
	include/libxls/ole.h
	include/libxls/xlsstruct.h
	include/libxls/xlstool.h
	include/libxls/xlstypes.h
	src/endian.c
	src/ole.c
	src/xls.c
	src/xlstool.c
)
set_property(TARGET xls PROPERTY C_STANDARD 99)
target_include_directories(xls 
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR} 
)

###################################################################

enable_testing()

find_file(XLS_TEST_FILE NAMES test2.xls PATHS test/files NO_DEFAULT_PATH)

configure_file(
	test/files.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/files.h
	@ONLY
)

add_executable(xlstest 
	test/files.h.in
	${CMAKE_CURRENT_BINARY_DIR}/files.h
	test/test.c
)
set_property(TARGET xlstest PROPERTY C_STANDARD 99)
target_include_directories(xlstest 
	PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
	PRIVATE include
) 
target_link_libraries(xlstest xls)
add_test(NAME ctest_xlstest COMMAND xlstest)

###################################################################

install(TARGETS xls EXPORT xls-targets DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT xls-targets FILE xlsConfig.cmake DESTINATION cmake)


