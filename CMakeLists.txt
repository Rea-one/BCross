cmake_minimum_required(VERSION 3.12)
project(BCross LANGUAGES CXX)

# 新增配置包含
include(${CMAKE_CURRENT_SOURCE_DIR}/config.cmake)

# 使用变量（删除硬编码）
set(BOOST_INCLUDE_DIR "${BOOST_ROOT}/include/boost-1_88")
set(BOOST_LIBRARY "${BOOST_ROOT}/lib/libboost_system-mgw14-mt-x64-1_88.a")
set(POSTGRESQL_INCLUDE_DIR "${POSTGRESQL_ROOT}/include")
set(POSTGRESQL_LIBRARY "${POSTGRESQL_ROOT}/lib/libpq.lib")
set(LIBPQXX_INCLUDE_DIR "${LIBPQXX_ROOT}/include")
set(LIBPQXX_LIBRARY "${LIBPQXX_ROOT}/build/src/LIBPQXX.a")
set(LIBPQXX_BUILD_INCLUDE_DIR "${LIBPQXX_ROOT}/build/include")

# 添加可执行文件（包含所有源文件）
add_executable(BCross
    src/main.cpp          # 主程序入口
    src/BCross.cpp
    src/config_reader.cpp
    src/listener.cpp
    src/IOthreads.cpp
    src/worker.cpp
)

# 包含头文件目录
target_include_directories(BCross PUBLIC
    include               # 当前项目的头文件目录
    ${POSTGRESQL_INCLUDE_DIR}
    ${LIBPQXX_INCLUDE_DIR}
    ${LIBPQXX_BUILD_INCLUDE_DIR}
    ${BOOST_INCLUDE_DIR}
)

# 链接库
target_link_libraries(BCross
    ${POSTGRESQL_LIBRARY}
    ${LIBPQXX_LIBRARY}
    ${BOOST_LIBRARY}
)