MACRO(COPY_IF_DIFFERENT FROM_DIR TO_DIR FILES TARGETS TAGS)
   # Macro to implement copy_if_different for a list of files
   # Arguments - 
   #   FROM_DIR - this is the source directory
   #   TO_DIR   - this is the destination directory
   #   FILES    - names of the files to copy 
   #              TODO: add globing. 
   #   TARGETS  - List of targets
   #   TAGS     - Since only the file name is used
   #              to generate rules, pre-pend a user 
   #              supplied tag to prevent duplicate rule errors. 
   SET(AddTargets "")
   FOREACH(SRC ${FILES})
       GET_FILENAME_COMPONENT(SRCFILE ${SRC} NAME) 
       # Command to copy the files to ${STEP1}/src, if changed.
       SET(TARGET  "${TAGS}/${SRCFILE}")
       IF("${FROM_DIR}" STREQUAL "")
           SET(FROM ${SRC})
       ELSE("${FROM_DIR}" STREQUAL "")
           SET(FROM ${FROM_DIR}/${SRC})
       ENDIF("${FROM_DIR}" STREQUAL "")        
       IF("${TO_DIR}" STREQUAL "")
           SET(TO ${SRCFILE})
       ELSE("${TO_DIR}" STREQUAL "")
           SET(TO   ${TO_DIR}/${SRCFILE})
       ENDIF("${TO_DIR}" STREQUAL "")
       ADD_CUSTOM_COMMAND(
           OUTPUT  ${TARGET}
           COMMAND ${CMAKE_COMMAND}
           ARGS    -E copy_if_different ${FROM} ${TO}
           COMMENT "Copying ${SRCFILE} ${TO_DIR}"
           )
       SET(AddTargets ${AddTargets} ${TARGET})
   ENDFOREACH(SRC ${FILES})
   SET(${TARGETS} ${AddTargets})
ENDMACRO(COPY_IF_DIFFERENT FROM_DIR TO_DIR FILES TARGETS TAGS)
