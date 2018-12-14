cmake_minimum_required(VERSION 2.8.8)
project(AnnotationTransferLib)
set( ANNOTATIONTRANSFERLIB "${PROJECT_NAME}-lib")
set( ANNOTATION_SRC ${MAINFOLDER}/Annotation/src/)
set( ANNOTATION_H ${MAINFOLDER}/Annotation/include/)
set( NANOFLANN ${MAINFOLDER}/nanoflann/)

set( Srcs
    ${ANNOTATION_SRC}/annotation
    ${ANNOTATION_SRC}/annotationutilities
    ${ANNOTATION_SRC}/facet
    ${ANNOTATION_SRC}/spthread
    ${ANNOTATION_SRC}/nanoflannhelper
)

set( Hdrs
    ${ANNOTATION_H}/annotation
    ${ANNOTATION_H}/annotationutilities
    ${ANNOTATION_H}/facet
    ${ANNOTATION_H}/spthread
    ${ANNOTATION_H}/nanoflannhelper
)

include_directories(
    ${ANNOTATION_H}/
    ${IMATISTLEX}/include/
    ${IMATISTL_H}/ImatiSTL/
    ${IMATISTL_H}/Kernel/
    ${IMATISTL_H}/TMesh/
    ${MAINFOLDER}/include/
    ${NANOFLANN}/include/
    ${NANOFLANN}/examples/
)

add_library(${ANNOTATIONTRANSFERLIB} ${Srcs} ${Hdrs})
target_include_directories(${ANNOTATIONTRANSFERLIB} PRIVATE ${ANNOTATION_H})
