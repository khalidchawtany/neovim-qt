
configure_file(version.h.in ${CMAKE_CURRENT_BINARY_DIR}/version.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(NEOVIM_QT_SOURCES
  auto/neovimapi0.cpp
  auto/neovimapi1.cpp
  auto/neovimapi2.cpp
  auto/neovimapi3.cpp
  auto/neovimapi4.cpp
  auto/neovimapi5.cpp
  auto/neovimapi6.cpp
  function.cpp
  msgpackiodevice.cpp
  msgpackrequest.cpp
  neovimconnector.cpp
  neovimconnectorhelper.cpp
  util.cpp)

if(WIN32)
  list(APPEND NEOVIM_QT_SOURCES stdinreader.cpp)
endif()


IF(APPLE)
   #SET(GUI_TYPE MACOSX_BUNDLE)
   INCLUDE_DIRECTORIES ( /System/Library/Frameworks/Foundation.framework/Versions/C/Headers)

   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(FOUNDATION_LIBRARY Foundation)

   MARK_AS_ADVANCED (COCOA_LIBRARY
       FOUNDATION_LIBRARY)

   SET(EXTRA_LIBS ${COCOA_LIBRARY} ${FOUNDATION_LIBRARY})

   list(APPEND NEOVIM_QT_SOURCES gui/OSXHideTitleBar.mm)
ENDIF (APPLE)


add_library(neovim-qt STATIC ${NEOVIM_QT_SOURCES})
target_link_libraries(neovim-qt Qt5::Network ${MSGPACK_LIBRARIES} ${EXTRA_LIBS})

add_subdirectory(gui)
