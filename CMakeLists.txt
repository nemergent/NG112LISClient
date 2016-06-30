cmake_minimum_required(VERSION 2.6)

project(LISClient)

find_package(CURL REQUIRED)
find_package (LibXml2 REQUIRED)

include_directories(${CURL_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR})
set(LIBS ${LIBS} ${CURL_LIBRARIES} ${LIBXML2_LIBRARIES})


set(SOURCE_FILES client.c held.c http.c xml.c)


add_executable(LISClient ${SOURCE_FILES})
target_link_libraries(LISClient ${LIBS})
