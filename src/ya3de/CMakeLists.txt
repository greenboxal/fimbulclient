file (GLOB BASE_FILES ya3de/*.c ya3de/*.cpp ya3de/*.h ya3de/*.hpp)

add_library (ya3de
    ${BASE_FILES}
)
target_link_libraries (ya3de ${Boost_LIBRARIES} ${SFML_LIBRARIES} ${GLEW_LIBRARY})
set_target_properties (ya3de PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
# cotire (ya3de)

source_group ("ya3de" FILES ${BASE_FILES})
