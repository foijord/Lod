cmake_minimum_required (VERSION 3.21.1)
project(Lod LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS YES CACHE BOOL "Export all symbols")

add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

if(WIN32)
	# target Windows >= 7 (Boost requires this)
	add_compile_options(-D _WIN32_WINNT=0x0601)
	# warning level 3, warnings as errors, big object files
	add_compile_options(/W3 /WX /bigobj /wd26812)
	# /Zi tells the compiler to put debug info into the OBJ file
	add_compile_options($<$<NOT:$<CONFIG:Debug>>:/Zi>)
	# /DEBUG tells the linker that we want to create a PDB file.
	add_link_options($<$<NOT:$<CONFIG:Debug>>:/DEBUG>)
	# /OPT:REF tells the linker to only put the functions that are referenced into the output binary.
	add_link_options($<$<NOT:$<CONFIG:Debug>>:/OPT:REF>)
	# /OPT:ICF tells the linker to look for functions that have identical assembly language code and only generate one of them
	add_link_options($<$<NOT:$<CONFIG:Debug>>:/OPT:ICF>)
endif(WIN32)

find_package(Boost REQUIRED)
find_package(Vulkan REQUIRED)
find_package(Threads REQUIRED)
find_package(glfw3 3.3 REQUIRED)

include_directories(
	$ENV{VULKAN_SDK}/Source/Volk
	${Boost_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/../glm/
	${CMAKE_SOURCE_DIR}/../gli/
	${CMAKE_SOURCE_DIR}/../)

link_directories(
	$ENV{VULKAN_SDK}/Lib
	${Boost_LIBRARY_DIRS}
	${CMAKE_BINARY_DIR})

set(TARGET Repl)

add_executable(${TARGET}
	Repl.cpp 
	ShaderReload.h
	../Innovator/Innovator.h
	../Innovator/Exceptions.h
	../Innovator/GliTextureImage.h
	../Innovator/GroupNodes.h
	../Innovator/LruCache.h
	../Innovator/Node.h
	../Innovator/Nodes.h
	../Innovator/Sensors.h
	../Innovator/SparseImageMemory.h
	../Innovator/State.h
	../Innovator/Surface.h
	../Innovator/Swapchain.h
	../Innovator/TileKey.h
	../Innovator/Timer.h
	../Innovator/Visitor.h
	../Innovator/VulkanBufferData.h
	../Innovator/VulkanDevice.h
	../Innovator/VulkanInstance.h
	../Innovator/VulkanObjects.h
	../Innovator/VulkanTextureImage.h
	../Innovator/Window.h
	../Innovator/Worker.h)

target_compile_definitions(${TARGET} PRIVATE
	GLFW_INCLUDE_VULKAN
	GLFW_INCLUDE_NONE
	VK_NO_PROTOTYPES
	VK_ENABLE_BETA_EXTENSIONS
	GLM_FORCE_LEFT_HANDED
	GLM_FORCE_DEPTH_ZERO_TO_ONE)

set_target_properties(${TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
	target_compile_definitions(${TARGET} PRIVATE VK_USE_PLATFORM_WIN32_KHR NOMINMAX)
else()
	target_compile_definitions(${TARGET} PRIVATE VK_USE_PLATFORM_XCB_KHR)
endif(WIN32)

target_link_libraries(${TARGET}
	${Boost_LIBRARIES}
	glfw
	Threads::Threads
	shaderc_shared
	${CMAKE_DL_LIBS})
