#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/GameManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/Character.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/KeyboardMovement.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/States/State.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/States/PauseState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/States/PlayingState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/States/MenuState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/States/EndState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/States/WinState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Timer/Timer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Menu.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/HealthBar/HealthBar.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/HealthBar/BossHealthBar.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/Enemy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/ShootingEnemy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/ChasingEnemy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/ShootingEnemy2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/ChasingEnemy2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Camera/Camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Map/TileMap.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Map/tinyxml2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/BossEnemy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/BossEnemy2.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Entities/Potion.cpp"


    )

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

