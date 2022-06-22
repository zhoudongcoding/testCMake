cmake_minimum_required(VERSION 3.10)

# set the project name
project(cheatmine VERSION 1.0)

# add the executable
add_executable(cheatmine main.cc)

target_include_directories(cheatmine PUBLIC
			"${PROJECT_BINARY_DIR}"
		        )