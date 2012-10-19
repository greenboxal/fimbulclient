
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(Content)
add_subdirectory(FileSystem)
add_subdirectory(Graphics)
add_subdirectory(GUI)
add_subdirectory(Scene)
add_subdirectory(System)

set(ya3de_source_files
# GLEW sources
  ../GL/glew.c
  
# SOIL sources
  ../SOIL/image_DXT.c
  ../SOIL/image_helper.c
  ../SOIL/SOIL.c
  ../SOIL/stb_image_aug.c

# YA3DE sources
  Config.cpp
  Game.cpp
  Logger.cpp
)

add_library(ya3de SHARED ${ya3de_source_files})
target_link_libraries(ya3de ya3de_content ya3de_filesystem ya3de_graphics ya3de_gui ya3de_scene ya3de_system ${OPENGL_LIBRARIES})

