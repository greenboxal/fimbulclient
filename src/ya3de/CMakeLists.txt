file (GLOB BASE_FILES YA3DE/*.c YA3DE/*.cpp YA3DE/*.h YA3DE/*.hpp)
file (GLOB GRAPHICS_FILES YA3DE/Graphics/*.c YA3DE/Graphics/*.cpp YA3DE/Graphics/*.h YA3DE/Graphics/*.hpp)
file (GLOB MATH_FILES YA3DE/Math/*.c YA3DE/Math/*.cpp YA3DE/Math/*.h YA3DE/Math/*.hpp)
file (GLOB FS_FILES YA3DE/FileSystem/*.c YA3DE/FileSystem/*.cpp YA3DE/FileSystem/*.h YA3DE/FileSystem/*.hpp)
file (GLOB CONTENT_FILES YA3DE/Content/*.c YA3DE/Content/*.cpp YA3DE/Content/*.h YA3DE/Content/*.hpp)
file (GLOB SCENE_FILES YA3DE/Scene/*.c YA3DE/Scene/*.cpp YA3DE/Scene/*.h YA3DE/Scene/*.hpp)

add_library (ya3de
    ${BASE_FILES}
	${GRAPHICS_FILES}
	${MATH_FILES}
	${FS_FILES}
	${CONTENT_FILES}
	${SCENE_FILES}
)
target_link_libraries (ya3de SOIL ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${GLEW_LIBRARY})
set_target_properties (ya3de PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
cotire (ya3de)

source_group ("YA3DE" FILES ${BASE_FILES})
source_group ("YA3DE\\Graphics" FILES ${GRAPHICS_FILES})
source_group ("YA3DE\\Math" FILES ${MATH_FILES})
source_group ("YA3DE\\FileSystem" FILES ${FS_FILES})
source_group ("YA3DE\\Content" FILES ${CONTENT_FILES})
source_group ("YA3DE\\Scene" FILES ${SCENE_FILES})
