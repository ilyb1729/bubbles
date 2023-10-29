cmake_minimum_required(VERSION 3.14)
project(glTest)

target_include_directories(imgui PUBLIC include)
target_include_directories(spdlog PUBLIC include)

set(sourceFiles
    main.cpp
)

add_executable(glTest ${sourceFiles} ${headerFiles})

# target_compile_options(glTest PUBLIC -Wall -Wextra -Werror)

target_link_libraries(glTest PRIVATE glad glfw imgui glm cgltf stb_image spdlog debugbreak fastgltf)
