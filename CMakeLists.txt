cmake_minimum_required(VERSION 3.16)
project(fitness-api LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(userver COMPONENTS core mongodb REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/handlers/auth_handlers.cpp
    src/handlers/user_handlers.cpp
    src/handlers/exercise_handlers.cpp
    src/handlers/workout_handlers.cpp
    src/storage/fitness_storage.cpp
    src/middlewares/auth_middleware.cpp
    src/utils/string_utils.cpp
    src/utils/http_utils.cpp
    src/domain/json.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE userver::core userver::mongodb fmt::fmt)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)