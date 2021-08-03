message(STATUS "Looking for XercesC for GDML support")
find_package(XercesC)
if (XercesC_FOUND)
  message(STATUS "Found XercesC version: ${XercesC_VERSION}")
  include_directories(SYSTEM ${XercesC_INCLUDE_DIR})
else()
  message(STATUS "Could not find XercesC - GDML disabled")
  set(USE_GDML OFF)
  unset(XercesC_LIBRARY)
  unset(XercesC_INCLUDE_DIR)
  unset(XercesC_VERSION)
endif()