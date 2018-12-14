cmake_minimum_required(VERSION 3.10)
project(statNLP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# compiler optimization, -O3 is the highest level.
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I /usr/local/include -O3")
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O3")

# include the dynet
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dynet)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Thunder-master)

# set the directory of eign3
set(EIGEN3_INCLUDE_DIR "/Users/ngs/Documents/cplusproject/eigen" )

IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
include_directories ( "${EIGEN3_INCLUDE_DIR}" )

# look for Boost
set(Boost_REALPATH ON)
find_package(Boost COMPONENTS program_options iostreams serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

#set(BLAS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Thunder-master)
#find_package(BLAS REQUIRED)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dynet/dynet)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Thunder-master/packages)

add_executable(statNLP_CPP main.cpp src/hybridnetworks/hypergraph.h
        src/hybridnetworks/global_network_param.h src/hybridnetworks/global_network_param.cpp
        src/hybridnetworks/feature_manager.h src/hybridnetworks/feature_manager.cpp src/hybridnetworks/local_network_param.h
        src/hybridnetworks/local_network_param.cpp src/example/LinearCRF/linear_crf_instance.h src/hybridnetworks/instance.h
        src/hybridnetworks/base_instance.h src/hybridnetworks/base_instance.h src/hybridnetworks/base_instance.cpp
        src/hybridnetworks/instance.cpp src/example/LinearCRF/linear_crf_instance.cpp src/hybridnetworks/network_model.h
        src/hybridnetworks/network_model.cpp src/hybridnetworks/data_manager.h src/hybridnetworks/data_manager.cpp
        src/example/LinearCRF/linear_crf_nework_compiler.h src/example/LinearCRF/linear_crf_network_compiler.cpp src/hybridnetworks/network_compiler.h
        src/hybridnetworks/network_compiler.cpp src/hybridnetworks/network.h src/hybridnetworks/network.cpp
        src/example/LinearCRF/linear_crf_network.h src/example/LinearCRF/linear_crf_network.cpp src/hybridnetworks/table_lookup_network.h
        src/hybridnetworks/table_lookup_network.cpp src/hybridnetworks/common.h src/hybridnetworks/network_id_manager.h
        src/hybridnetworks/network_id_manager.cpp src/example/LinearCRF/linear_crf_feature_manager.h src/example/LinearCRF/linear_crf_feature_manager.cpp
        src/hybridnetworks/feature_array.h src/hybridnetworks/feature_array.cpp src/hybridnetworks/discriminative_network_model.h
        src/hybridnetworks/discriminative_network_model.cpp src/hybridnetworks/local_network_learner_thread.h
        src/hybridnetworks/local_network_learner_thread.cpp src/common/opt/lbfgs.h src/common/opt/lbfgs.cpp
        src/hybridnetworks/freelistbase.h src/hybridnetworks/freelistbase.cpp src/hybridnetworks/local_network_decoder_thread.h
        src/hybridnetworks/local_network_decoder_thread.cpp  src/common/opt/math_calc.h src/common/opt/math_calc.cpp src/example/TreeCRF/tree_crf_instance.h src/example/TreeCRF/tree_crf_instance.cpp src/example/TreeCRF/binary_tree.h src/example/TreeCRF/binary_tree.cpp src/example/TreeCRF/node.h src/example/TreeCRF/node.cpp src/example/TreeCRF/binary_tree_common.h src/example/TreeCRF/tree_crf_feature_manager.h src/example/TreeCRF/tree_crf_feature_manager.cpp src/example/TreeCRF/tree_crf_nework_compiler.h src/example/TreeCRF/tree_crf_nework_compiler.cpp src/example/TreeCRF/Lables.h src/example/TreeCRF/Lables.cpp src/example/TreeCRF/cnf_rules.h src/example/TreeCRF/cnf_rules.cpp src/example/TreeCRF/tree_crf_network_compiler.h src/example/TreeCRF/tree_crf_network_compiler.cpp src/example/TreeCRF/tree_crf_network.h src/example/TreeCRF/tree_crf_network.cpp src/hybridnetworks/builder.h)


MESSAGE("-- Enabling SWIG")
add_subdirectory(swig)

# link statnlp and dynet
#target_link_libraries(statNLP cnn)
#target_link_libraries(statNLP dynet libblas.a)
#target_link_libraries(statNLP dynet)
