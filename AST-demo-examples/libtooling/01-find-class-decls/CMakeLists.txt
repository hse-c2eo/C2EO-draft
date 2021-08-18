cmake_minimum_required(VERSION 3.20)

project(find-class-decls)

set(PATH_TO_LLVM_SOURCE "/home/al/llvm-clang/")
set(PATH_TO_LLVM_BUILD "/home/al/llvm-clang/build")
 
include_directories("${PATH_TO_LLVM_SOURCE}/clang/include")
include_directories("${PATH_TO_LLVM_SOURCE}/llvm/include")
 
include_directories("${PATH_TO_LLVM_BUILD}/tools/clang/include")
include_directories("${PATH_TO_LLVM_BUILD}/include")
 
link_directories("${PATH_TO_LLVM_BUILD}/lib/")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fno-rtti)
 
add_executable(find-class-decls main.cpp)
 
target_link_libraries(find-class-decls
  PRIVATE
  clangAST
  clangBasic
  clangFrontend
  clangSerialization
  clangTooling
  LLVMSupport
  )

#set(LLVM_LINK_COMPONENTS
#  Support
#  )

#  llvm_config
