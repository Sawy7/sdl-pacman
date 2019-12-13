cmake_minimum_required(VERSION 3.4)
project(sdl)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")

find_package(SDL2)
if (SDL2_FOUND)
    message("SDL2 FOUND")
    add_executable(sdl sdl.c pacman.c)
    target_include_directories(sdl PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(sdl ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} SDL2_image SDL2_ttf)
endif()


#gcc sdl.c pacman.c -o sdl -lSDL2 -lSDL2_image -lSDL2_ttf
