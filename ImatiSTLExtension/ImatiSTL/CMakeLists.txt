cmake_minimum_required(VERSION 2.8.8)

project(ImatiSTL)
set( IMATISTLLIB "${PROJECT_NAME}-lib")

set( ALGORITHMS ${IMATISTL}/src/Algorithms )
set( IMATISTL_SRC ${IMATISTL}/src/ImatiSTL )
set( KERNEL ${IMATISTL}/src/Kernel )
set( TMESH ${IMATISTL}/src/TMesh )
set( IMATISTL_H ${IMATISTL}/include/ImatiSTL )
set( KERNEL_H ${IMATISTL}/include/Kernel )
set( TMESH_H ${IMATISTL}/include/TMesh )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -w")

# Set your files and resources here
set( Srcs 
    ${ALGORITHMS}/checkAndRepair
    ${ALGORITHMS}/delaunize
    ${ALGORITHMS}/detectIntersections
    ${ALGORITHMS}/faceClustering
    ${ALGORITHMS}/featureRecover
    ${ALGORITHMS}/holeFilling
    ${ALGORITHMS}/simplification
    ${ALGORITHMS}/smoothing
    ${ALGORITHMS}/spherize
    ${ALGORITHMS}/subdivision
    ${IMATISTL_SRC}/computeOuterHull
    ${IMATISTL_SRC}/cutIntersections
    ${IMATISTL_SRC}/dijkstraGraph
    ${IMATISTL_SRC}/epsilonSampling
    ${IMATISTL_SRC}/marchIntersections
    ${IMATISTL_SRC}/offset
    ${IMATISTL_SRC}/uniformRemesh
    ${KERNEL}/coordinates
    ${KERNEL}/graph
    ${KERNEL}/heap
    ${KERNEL}/jqsort
    ${KERNEL}/list
    ${KERNEL}/matrix
    ${KERNEL}/orientation
    ${KERNEL}/point
    ${KERNEL}/tmesh
    ${TMESH}/edge
    ${TMESH}/edgeHeap
    ${TMESH}/io
    ${TMESH}/tin
    ${TMESH}/triangle
    ${TMESH}/vertex
)

set( Hdrs
    ${IMATISTL_H}/imatistl
    ${IMATISTL_H}/trimesh
    ${IMATISTL_H}/dijkstraGraph
    ${IMATISTL_H}/marchIntersections
    ${IMATISTL_H}/cutIntersections
    ${KERNEL_H}/basics
    ${KERNEL_H}/graph
    ${KERNEL_H}/jqsort
    ${KERNEL_H}/matrix
    ${KERNEL_H}/tmesh_kernel
    ${KERNEL_H}/coordinates
    ${KERNEL_H}/heap
    ${KERNEL_H}/list
    ${KERNEL_H}/point
    ${TMESH_H}/detectIntersections
    ${TMESH_H}/edgeHeap
    ${TMESH_H}/simplification
    ${TMESH_H}/tmesh
    ${TMESH_H}/vertex
    ${TMESH_H}/edge
    ${TMESH_H}/faceClustering
    ${TMESH_H}/tin
    ${TMESH_H}/triangle
)

include_directories(
    ${IMATISTL_H}/
    ${KERNEL_H}/
    ${TMESH_H}/
)

add_library(${IMATISTLLIB} ${Srcs} ${Hdrs})
target_include_directories(${IMATISTLLIB} PRIVATE
    ${IMATISTL_H}/
    ${KERNEL_H}/
    ${TMESH_H}/
)
set_target_properties(${IMATISTLLIB} PROPERTIES LINKER_LANGUAGE CXX)
