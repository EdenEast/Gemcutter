list(APPEND font_encoder_files
	"main.cpp"
	"FontEncoder.h"
	"FontEncoder.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${font_encoder_files})

add_executable(font_encoder ${font_encoder_files})
sf_target_compile_warnings(font_encoder)
sf_target_compile_warnings_as_errors(font_encoder OPTIONAL)

target_link_libraries(font_encoder
	PRIVATE
		gemcutter
		freetype
)

# Copy the required .DLLs next to the executable.
get_target_property(FREETYPE_BINARIES_DIR freetype INTERFACE_LINK_DIRECTORIES)
get_target_property(FONT_ENCODER_BINARY_DIR font_encoder BINARY_DIR)

add_custom_command(TARGET font_encoder POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy "${FREETYPE_BINARIES_DIR}/freetype6.dll" "${FONT_ENCODER_BINARY_DIR}/Release/"
	COMMAND ${CMAKE_COMMAND} -E copy "${FREETYPE_BINARIES_DIR}/zlib1.dll" "${FONT_ENCODER_BINARY_DIR}/Release/"
	VERBATIM
)