include_directories (.)

file (GLOB BASE_FILES fclient/*.c fclient/*.cpp fclient/*.h fclient/*.hpp)
file (GLOB FS_FILES fclient/FileSystem/*.c fclient/FileSystem/*.cpp fclient/FileSystem/*.h fclient/FileSystem/*.hpp)
file (GLOB GRF_FILES fclient/FileSystem/Grf/*.c fclient/FileSystem/Grf/*.cpp fclient/FileSystem/Grf/*.h fclient/FileSystem/Grf/*.hpp)
file (GLOB ZLIB_FILES fclient/FileSystem/zlib/*.c fclient/FileSystem/zlib/*.cpp fclient/FileSystem/zlib/*.h fclient/FileSystem/zlib/*.hpp)
file (GLOB GRAPHICS_FILES fclient/Graphics/*.c fclient/Graphics/*.cpp fclient/Graphics/*.h fclient/Graphics/*.hpp)

add_executable (fclient
    ${BASE_FILES}
	${FS_FILES}
	${GRF_FILES}
	${ZLIB_FILES}
	${GRAPHICS_FILES}
)
target_link_libraries (fclient ya3de ${AWESOMIUM_LIBRARY})
set_target_properties (fclient PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
# cotire (fclient)

source_group ("fclient" FILES ${BASE_FILES})
source_group ("fclient\\FileSystem" FILES ${FS_FILES})
source_group ("fclient\\FileSystem\\Grf" FILES ${GRF_FILES})
source_group ("fclient\\FileSystem\\zlib" FILES ${ZLIB_FILES})
source_group ("fclient\\Graphics" FILES ${GRAPHICS_FILES})
