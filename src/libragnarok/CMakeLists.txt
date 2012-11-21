include_directories (.)

file (GLOB FS_FILES Ragnarok/FileSystem/*.c Ragnarok/FileSystem/*.cpp Ragnarok/FileSystem/*.h Ragnarok/FileSystem/*.hpp)
file (GLOB GRF_FILES Ragnarok/FileSystem/Grf/*.c Ragnarok/FileSystem/Grf/*.cpp Ragnarok/FileSystem/Grf/*.h Ragnarok/FileSystem/Grf/*.hpp)
file (GLOB ZLIB_FILES Ragnarok/FileSystem/zlib/*.c Ragnarok/FileSystem/zlib/*.cpp Ragnarok/FileSystem/zlib/*.h Ragnarok/FileSystem/zlib/*.hpp)
file (GLOB GRAPHICS_FILES Ragnarok/Graphics/*.c Ragnarok/Graphics/*.cpp Ragnarok/Graphics/*.h Ragnarok/Graphics/*.hpp)

add_library (ragnarok
	${FS_FILES}
	${GRF_FILES}
	${ZLIB_FILES}
	${GRAPHICS_FILES}
)
target_link_libraries (ragnarok ya3de)
set_target_properties (ragnarok PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
# cotire (Ragnarok)

source_group ("Ragnarok\\FileSystem" FILES ${FS_FILES})
source_group ("Ragnarok\\FileSystem\\Grf" FILES ${GRF_FILES})
source_group ("Ragnarok\\FileSystem\\zlib" FILES ${ZLIB_FILES})
source_group ("Ragnarok\\Graphics" FILES ${GRAPHICS_FILES})
