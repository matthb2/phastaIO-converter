CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)
PROJECT(PHASTA C CXX Fortran )
enable_language(Fortran)
enable_language(C)
enable_language(CXX)

list(APPEND CMAKE_MODULE_PATH ${PHASTA_SOURCE_DIR}/CMakeFiles)

#Handle Fortran name mangling
include(FortranCInterface)
FortranCInterface_VERIFY(CXX)
FortranCInterface_Header(FCMangle.h SYMBOLS write_restart write_error
write_displ write_field write_hessian)
include_directories("${PHASTA_BINARY_DIR}")

IF(${CMAKE_BUILD_TYPE} MATCHES "Debug" )
  ADD_DEFINITIONS( -DDEBUG )
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug" )

# set archos "like" options for things like c calling fortran bindings
# later on we may want to do it dependent on behavior (e.g. 
# C_FORTRAN_CALL == uppercase, underline, nochange)
#OPTION(ARCHOS_OPTION "ArchOS dependent defines (e.g. -DLINUX)" )
SET(ARCHOS_OPTION "-DLINUX" CACHE STRING "ArchOS dependent defines (e.g. -DLINUX)" )
ADD_DEFINITIONS( ${ARCHOS_OPTION} )


SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PHASTA_BINARY_DIR}/lib)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PHASTA_BINARY_DIR}/bin)

OPTION(PHASTA_USE_MPI "Build MPI components" ON)
IF(PHASTA_USE_MPI)
  ADD_DEFINITIONS( -DPARALLEL -DMPI -DMPICH_SKIP_MPICXX )
  FIND_PACKAGE(MPI REQUIRED)
#  SET(CMAKE_MODULE_PATH ${PHASTA_SOURCE_DIR}/CMakeFiles)
  # bug in FIND_PACKAGE for Threads -- ignore for now (bug has been reported)...
  FIND_PACKAGE(Threads)
  IF(Threads_FOUND)
    INCLUDE(FindThreads)
  ENDIF(Threads_FOUND)
ENDIF(PHASTA_USE_MPI)

IF(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
endif()
IF(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
endif()
IF(CMAKE_C_COMPILER_ID MATCHES Clang)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
endif()
IF(CMAKE_C_COMPILER_ID MATCHES GNU)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra")
endif()


add_subdirectory(phastaIO)
include_directories("${PHASTA_SOURCE_DIR}/phastaIO")

add_subdirectory(converterIO)
add_subdirectory(syncio-tester)
