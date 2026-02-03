cmake_minimum_required(VERSION 3.23)
project(webserv VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Disable compiler extensions (e.g., prevents -std=gnu++98, forces -std=c++98)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${SOURCES}
        src/http/Response.hpp
        src/http/MimeTypes.hpp
        src/http/CgiHandler.hpp
        src/http/AutoIndex.hpp
        src/RequestHandler.hpp
        src/http/HttpError.hpp
        src/http/FileToUpload.hpp
        src/http/RedirectResponse.hpp
        src/http/StaticFileResponse.hpp
        src/http/ErrorPageGenerator.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

set(RESSOURCES_DIR
        configs
        cgi
        pages
        storing
)

foreach(DIR ${RESSOURCES_DIR})
    add_custom_command(
            TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}
            ${CMAKE_CURRENT_BINARY_DIR}/${DIR}
            COMMENT "Link de ${CMAKE_CURRENT_SOURCE_DIR}/${DIR} vers ${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
    )
endforeach()
