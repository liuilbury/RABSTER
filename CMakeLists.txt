cmake_minimum_required(VERSION 3.0)
project (RABSTER)
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
set(HTML src/html/node.h src/html/node.cpp src/html/HtmlContent.cpp src/html/HtmlContent.h src/html/Handle.h  src/html/HtmlLayout.cpp src/html/HtmlLayout.h src/html/DomNode.cpp src/html/DomNode.h src/render/RenderNode.cpp src/render/RenderNode.h)

add_executable(rabster src/Rabster.cpp ${CSS} ${HTML} ${gumbo} ${yoga} src/render/Style.cpp src/render/Style.h src/render/Style_Layout.cpp src/render/Style_Layout.h src/render/Style_Color.cpp src/render/Style_Color.h src/render/Render.cpp src/render/Render.h include/html/Node.h)
target_link_libraries(rabster  PUBLIC gdiplus)
target_link_libraries(rabster PUBLIC iconv)
target_include_directories(rabster PUBLIC /include/parserutils/iconv.h)
#add_definitions(-DDEBUG_HTML)
