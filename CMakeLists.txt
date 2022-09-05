
project (ftp_server)

aux_source_directory(./src/ SRC)

add_executable(server ${SRC})

target_link_libraries(server ACE)