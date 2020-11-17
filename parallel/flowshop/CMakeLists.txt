set(YEWPAR_BUILD_BNB_APPS_FLOWSHOP "ON" CACHE BOOL "Build Branch and Bound Flowshop")
if (YEWPAR_BUILD_BNB_APPS_FLOWSHOP)
add_hpx_executable(flowshop
  SOURCES main.cpp
  COMPILE_FLAGS ""
  DEPENDENCIES YewPar_lib)
endif(YEWPAR_BUILD_BNB_APPS_MAXCLIQUE)
