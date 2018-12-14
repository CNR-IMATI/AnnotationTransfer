cmake_minimum_required(VERSION 2.8.8)

project(AnnotationTransfer)
set(ERROR FALSE)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
if(CMAKE_USE_PTHREADS_INIT)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive -w -pthread ${CMAKE_CXX_FLAGS}")
    else()
        set(ERROR TRUE)
    endif()
else()
    set(ERROR TRUE)
endif()

if(ERROR)
    message(FATAL_ERROR "Some functionalities can't be used without c++>=11 or boost. Please use a different C++ compiler or install boost. See readme file in the main folder.")
endif()

set( MAINFOLDER ${CMAKE_SOURCE_DIR} )
set( ANNOTATION ${MAINFOLDER}/Annotation )
set( ANNOTATION_H ${ANNOTATION}/include )
set( IMATISTLEX ${MAINFOLDER}/ImatiSTLExtension )
set( IMATISTLEX_H ${IMATISTLEX}/include/ )
set( IMATISTL_H ${IMATISTLEX}/ImatiSTL/include/ )
set( NANOFLANN ${MAINFOLDER}/nanoflann/)

add_subdirectory(${IMATISTLEX})
add_subdirectory(${ANNOTATION})
# Set your files and resources here

include_directories(
  ${MAINFOLDER}/include/
  ${ANNOTATION_H}/
  ${IMATISTLEX_H}/
  ${IMATISTL_H}/ImatiSTL/
  ${IMATISTL_H}/Kernel/
  ${IMATISTL_H}/TMesh/
  ${NANOFLANN}/include/
  ${NANOFLANN}/examples/
)

set( Srcs ${MAINFOLDER}/src/main ${MAINFOLDER}/include/mainthread)
set( Docs ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_executable(${PROJECT_NAME} ${Srcs} ${Docs} ${UI_Srcs} ${MOC_Hdrs} ${QRC_Srcs})
target_link_libraries(${PROJECT_NAME} AnnotationTransferLib-lib ImatiSTLExtension-lib)
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)
