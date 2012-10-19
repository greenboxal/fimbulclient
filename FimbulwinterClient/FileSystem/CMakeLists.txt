
set(fcragnarok_filesystem_source_files
  GrfFileSystem.cpp
  Grf/Grf.cpp
  Grf/GrfCrypt.cpp

# Zlib, maybe migrate to external lib?
  Grf/zlib/adler32.c
  Grf/zlib/compress.c
  Grf/zlib/crc32.c
  Grf/zlib/deflate.c
  Grf/zlib/infback.c
  Grf/zlib/inffast.c
  Grf/zlib/inflate.c
  Grf/zlib/inftrees.c
  Grf/zlib/trees.c
  Grf/zlib/uncompr.c
  Grf/zlib/zutil.c
)

add_library(fcragnarok_filesystem ${fcragnarok_filesystem_source_files})

