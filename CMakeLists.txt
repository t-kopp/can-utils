cmake_minimum_required(VERSION 3.3)

project(can-utils LANGUAGES C)

include (GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  add_definitions(-D_GNU_SOURCE)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-parentheses")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-strict-aliasing")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSO_RXQ_OVFL=40")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPF_CAN=29")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DAF_CAN=PF_CAN")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DN_SLCAN=17")

include_directories (.)
include_directories (./include)

set(PROGRAMS_CANLIB
    asc2log
    canbusload
    candump
    cangen
    canlogserver
    canplayer
    cansend
    log2asc
    log2long
)

set(PROGRAMS_J1939
    jacd
    jcat
    jspy
    jsr
    testj1939
)

set(PROGRAMS_CANLIB_RECVMMSG
    canpump
)

set(PROGRAMS
    ${PROGRAMS_CANLIB}
    bcmserver
    can-calc-bit-timing
    canecho
    canfdtest
    cangw
    cansniffer
    gwtest
    isotpdump
    isotpperf
    isotprecv
    isotpsend
    isotpserver
    isotpsniffer
    isotptun
    slcan_attach
    slcand
    slcanpty
    tst-bcm-cycle
    tst-bcm-dump
    tst-bcm-filter
    tst-bcm-rtr
    tst-bcm-rx-sendto
    tst-bcm-single
    tst-bcm-throttle
    tst-bcm-tx-sendto
    tst-bcm-tx_delete
    tst-bcm-tx_read
    tst-bcmfd-cycle
    tst-bcmfd-filter
    tst-err
    tst-filter
    tst-filter-master
    tst-filter-server
    tst-packet
    tst-proc
    tst-raw
    tst-raw-filter
    tst-raw-sendto
    tst-rcv-own-msgs
)

if(NOT ANDROID)
  list(APPEND PROGRAMS ${PROGRAMS_J1939})
  list(APPEND PROGRAMS ${PROGRAMS_CANLIB_RECVMMSG})
  list(APPEND PROGRAMS_CANLIB ${PROGRAMS_CANLIB_RECVMMSG})

  add_library(j1939 STATIC
      libj1939.c
  )
endif()

add_library(can STATIC
    lib.c
    canframelen.c
)

foreach(name ${PROGRAMS})
  add_executable(${name} ${name}.c)

  if("${name}" IN_LIST PROGRAMS_J1939)
    target_link_libraries(${name}
        PRIVATE j1939
    )
  elseif("${name}" IN_LIST PROGRAMS_CANLIB)
    target_link_libraries(${name}
        PRIVATE can
    )
  endif()

  install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_BINDIR})
endforeach()


ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_SOURCE_DIR}/cmake/make_uninstall.cmake")
