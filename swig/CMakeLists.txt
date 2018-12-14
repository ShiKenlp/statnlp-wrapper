##cmakelist.txt for python wrapper.

INCLUDE_DIRECTORIES(../) # for c++ cmakelist.txt.
include_directories(src/hybridnetworks/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall -Wextra -Wno-unused-parameter -fPIC")

# add t_inherit.cpp into project library.
add_library(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/hybridnetworks/global_network_param.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/feature_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/local_network_param.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/base_instance.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/instance.cpp
        ${PROJECT_SOURCE_DIR}/src/example/LinearCRF/linear_crf_instance.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/network_model.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/data_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/example/LinearCRF/linear_crf_network_compiler.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/network_compiler.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/network.cpp
        ${PROJECT_SOURCE_DIR}/src/example/LinearCRF/linear_crf_network.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/table_lookup_network.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/network_id_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/example/LinearCRF/linear_crf_feature_manager.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/feature_array.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/discriminative_network_model.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/local_network_learner_thread.cpp
        ${PROJECT_SOURCE_DIR}/src/common/opt/lbfgs.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/freelistbase.cpp
        ${PROJECT_SOURCE_DIR}/src/hybridnetworks/local_network_decoder_thread.cpp
        ${PROJECT_SOURCE_DIR}/src/common/opt/math_calc.cpp )

#set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/src/hybridnetworks/instance.h")

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        PUBLIC_HEADER DESTINATION include)

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

set(Python_ADDITIONAL_VERSIONS 3.6)
FIND_PACKAGE(PythonLibs 3 REQUIRED)

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH}) #for python.

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(statnlp.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(statnlp.i PROPERTIES SWIG_FLAGS "-python")
SET_SOURCE_FILES_PROPERTIES(statnlp.i PROPERTIES SWIG_FLAGS "-threads")

swig_add_module(statnlp python statnlp.i)
swig_link_libraries(statnlp ${PROJECT_NAME} ${PYTHON_LIBRARIES})

# Files to install with Python
set(PYTHON_INSTALL_FILES
        ${CMAKE_CURRENT_BINARY_DIR}/statnlp.py
        ${CMAKE_CURRENT_BINARY_DIR}/_statnlp.so)
