
aux_source_directory(. SRC_LISTS)
aux_source_directory(utils SRC_LISTS)
aux_source_directory(box SRC_LISTS)
aux_source_directory(parser SRC_LISTS)


set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Lib)

add_library(mp4 ${SRC_LISTS})
