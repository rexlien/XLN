set(EXE_NAME TestBed)

include(${PROJECT_SOURCE_DIR}/cmake/util.cmake)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Core)
include_directories(${PROJECT_SOURCE_DIR}/UniEngine)

add_executable(
  ${EXE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.mm
)

target_link_libraries(${EXE_NAME}
    "-framework AVFoundation"
    "-framework CoreGraphics"
    "-framework CoreMotion"
    "-framework Foundation"
    "-framework MediaPlayer"
    "-framework OpenGLES"
    "-framework QuartzCore"
    "-framework UIKit"
  )

#set_target_properties(NavApp3 PROPERTIES
#  MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourcompany.NavApp3"
#  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in
#  RESOURCE "${RESOURCES}"
#  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
#  XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
#  XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER YES
#  XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${CMAKE_CURRENT_LIST_DIR}/NavApp3_Prefix.pch
#  XCODE_ATTRIBUTE_INFOPLIST_PREPROCESS YES
#  XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 3.0
#  )

#combine_static_lib(UniEngine Core)
if(IOS)
set_xcode_property_simple(${EXE_NAME} GCC_GENERATE_DEBUGGING_SYMBOLS YES)
set_target_properties(${EXE_NAME} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
endif()

target_link_libraries(${EXE_NAME} UniEngine)
#target_link_libraries(${EXE_NAME} Core)
#combine_static_lib(UniEngine Core)



#target_link_libraries(${EXE_NAME} ${THIRD_PARTY_LIBS})
#target_link_libraries(${EXE_NAME} BOOST_LIB) 
target_link_libraries(${EXE_NAME} CurlLib) 

#print_target_properties(CurlLib)
#print_target_properties(UniEngine)

#set_target_properties(${LIB_NAME} PROPERTIES COMPILE_DEFINITIONS BUILDING_LIBCURL)

# Remove the "lib" prefix since the library is already named "libcurl".
#set_target_properties(${LIB_NAME} PROPERTIES PREFIX "")
#set_target_properties(${LIB_NAME} PROPERTIES IMPORT_PREFIX "")

