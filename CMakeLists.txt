cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_CUDA_ARCHITECTURES 60-real)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17")

project(example_o3d LANGUAGES C CXX CUDA)

add_compile_options(-std=c++17)

file(GLOB_RECURSE SRC_SRCS "src/*.cpp")
file(GLOB_RECURSE SRC_INCS "src/*.h")

add_library(${PROJECT_NAME}_lib STATIC ${SRC_SRCS} ${SRC_INCS})

if(DEFINED ENV{CUDA_TOOLKIT})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{CUDA_TOOLKIT})
endif()

find_package(CUDAToolkit REQUIRED)
if(${CUDAToolkit_FOUND})
  target_link_libraries(${PROJECT_NAME}_lib PUBLIC CUDA::cudart CUDA::cuda_driver)
  target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CUDA_INCLUDE_DIRS})
endif()

find_package(Open3D REQUIRED)
if(${Open3D_FOUND})
  target_link_libraries(${PROJECT_NAME}_lib PUBLIC Open3D::Open3D)
  include_directories(/usr/local/include/open3d/3rdparty)
endif()
  
find_package(catkin REQUIRED COMPONENTS roscpp eigen_conversions tf2_geometry_msgs tf2_ros open3d_conversions)
if(${catkin_FOUND})
  catkin_package(
    CATKIN_DEPENDS roscpp eigen_conversions tf2_geometry_msgs tf2_ros open3d_conversions
    DEPENDS Open3D
    )

  target_include_directories(${PROJECT_NAME}_lib PUBLIC ${catkin_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${catkin_LIBRARIES})
  add_dependencies(${PROJECT_NAME}_lib ${catkin_EXPORTED_TARGETS})
endif()
    
add_library(CUDAo3dLib STATIC src/My_O3D_GPU.cu)
set_target_properties(CUDAo3dLib PROPERTIES  LANGUAGE CUDA CUDA_SEPARABLE_COMPILATION ON CUDA_RESOLVE_DEVICE_SYMBOLS ON)
target_compile_options(CUDAo3dLib PRIVATE $<$<COMPILE_LANGUAGE:CUDA>: -dc --extended-lambda --expt-relaxed-constexpr >)

target_link_libraries(CUDAo3dLib PUBLIC CUDA::cuda_driver)
# target_compile_definitions(CUDAo3dLib PUBLIC BUILD_CUDA_MODULE) # Uncomment to a succesful compilation
target_link_libraries(${PROJECT_NAME}_lib PUBLIC CUDAo3dLib)

add_executable(${PROJECT_NAME}_node src/main.cpp)
target_link_libraries(${PROJECT_NAME}_node ${PROJECT_NAME}_lib)