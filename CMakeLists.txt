cmake_minimum_required(VERSION 3.0)
project (RABSTER)
link_directories(${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")


include_directories(src/css/libparserutils/0.2.4/src)
file(GLOB_RECURSE file5    src/css/libparserutils/0.2.4/src/*.c)
file(GLOB_RECURSE file6    src/css/libparserutils/0.2.4/src/*.h)


include_directories(src/css/libwapcaplet/0.4.3/src)
file(GLOB_RECURSE file3    src/css/libwapcaplet/0.4.3/src/*.c)
file(GLOB_RECURSE file4    src/css/libwapcaplet/0.4.3/src/*.h)


include_directories(src/css/libcss/0.9.1/src)
file(GLOB_RECURSE file1    src/css/libcss/0.9.1/src/*.c)
file(GLOB_RECURSE file2    src/css/libcss/0.9.1/src/*.h)


include_directories(src/yoga)
aux_source_directory(./src/yoga yoga)
aux_source_directory(./src/yoga/event yoga)


include_directories(src/gumbo)
aux_source_directory(./src/gumbo gumbo)


include_directories(${PROJECT_SOURCE_DIR}/include)
set(CSS  ${file1} ${file2} ${file3} ${file4} ${file5} ${file6})
set(HTML src/html/node.h src/html/node.cpp src/html/HtmlContent.cpp src/html/HtmlContent.h src/html/Handle.h)
add_executable(demo_html src/demo_html.cpp ${CSS} ${HTML} ${gumbo} ${yoga})
target_link_libraries(demo_html PUBLIC glfw3)
target_link_libraries(demo_html PUBLIC iconv)
target_include_directories(demo_html PUBLIC /include/iconv.h)


add_executable(demo_opengl src/demo_opengl.cpp src/glad.c)
target_link_libraries(demo_opengl PUBLIC glfw3)
target_link_libraries(demo_opengl PUBLIC iconv)
target_include_directories(demo_opengl PUBLIC /include/iconv.h)


add_executable(demo_all src/demo_all.cpp src/glad.c ${CSS} ${HTML})
target_link_libraries(demo_all PUBLIC glfw3)
target_link_libraries(demo_all PUBLIC iconv)
target_include_directories(demo_all PUBLIC /include/iconv.h)

add_executable(rabster src/Rabster.cpp ${CSS} ${HTML} ${gumbo} ${yoga})
target_link_libraries(rabster  PUBLIC gdiplus.lib)
target_link_libraries(rabster PUBLIC iconv)
target_include_directories(rabster PUBLIC /include/iconv.h)


add_executable(jsontest jsontest.cpp)
#add_definitions(-DDEBUG_HTML)
