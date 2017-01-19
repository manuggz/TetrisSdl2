cmake_minimum_required(VERSION 3.6)
project(Tetris)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES_BB src/main.cpp src/engine/sprites/animacion/animacion.cpp src/engine/mapa/TileEnLlamas.cpp  src/engine/sprites/animacion/Control_Animacion.cpp  src/engine/util/CMetaData.cpp   src/engine/util/galeria.cpp src/engine/GameManager/GameManager.cpp  src/engine/interfaces/InterfazGrafica.hpp src/engine/util/util.cpp src/engine/sprites/CGroup.cpp src/engine/sprites/CSprite.cpp src/engine/layout/Componentes/BotonComponent.hpp src/engine/util/LTexture.cpp src/engine/util/LTexture.hpp src/engine/util/LTimer.cpp src/engine/util/LTimer.hpp src/engine/util/CFont.hpp src/engine/util/CFont.cpp  src/engine/layout/Componentes/LabelComponent.hpp src/engine/layout/LayoutManager/LayoutVertical.hpp src/engine/layout/LayoutManager/LayoutComponent.hpp  src/engine/interfaces/MenuListLabel.hpp  src/engine/layout/LayoutManager/LayoutAbsolute.hpp src/engine/layout/Componentes/Component.hpp src/engine/layout/LayoutManager/LayoutParent.hpp src/engine/layout/Componentes/ImageComponent.hpp src/engine/sprites/CDrawGroup.hpp src/engine/sprites/CUpdateGroup.hpp src/engine/util/SpriteSheet.hpp src/engine/mapa/include/TMXParser.cpp src/engine/mapa/include/TMXParser.h src/engine/mapa/include/rapidxml/rapidxml.hpp  src/engine/mapa/include/rapidxml/rapidxml_utils.hpp  src/engine/mapa/include/rapidxml/rapidxml_print.hpp  src/engine/mapa/include/rapidxml/rapidxml_iterators.hpp   src/engine/mapa/CMapa.hpp  src/engine/interfaces/PopUpInterfaz.hpp src/engine/util/CColor.hpp src/engine/interfaces/PopUpMostrarMensajeTextoTimer.hpp src/engine/interfaces/PopUpCountDown.hpp src/engine/mapa/TileBloqueRompibleAnimado.hpp src/engine/sprites/CSpriteCuadro.hpp src/engine/interfaces/MenuListLabel.cpp src/engine/interfaces/PopUpInterfaz.cpp src/engine/sprites/CUpdateGroup.cpp src/engine/sprites/CDrawGroup.cpp src/engine/util/EfectoSonido.hpp src/engine/sprites/SpriteContainer.hpp src/engine/interfaces/GameManagerInterfazUI.hpp src/engine/interfaces/GameManagerPopUpInterfaz.hpp src/engine/util/MusicaFondo.hpp src/engine/util/Toast.hpp src/AnimacionDesvanecerOut.hpp src/ControlaAnimacion.hpp src/ControlaAnimacionInterfaz.hpp src/ControlTeclasPlayer.hpp src/Menu.hpp src/RandomGenerator.hpp src/SimpleAnimacion.hpp src/TetrisInterfaz.hpp src/TetrisJuego.hpp src/Tetromino.hpp src/engine/util/BitmapFont.hpp)

set(SDL2NET_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2_net.so)

add_executable(Tetris ${SOURCE_FILES_BB})

INCLUDE(FindPkgConfig)

#src/[^e^n][a-z]*/([a-z]*/)*[a-z]*(.hpp|.cpp)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2IMIXER REQUIRED SDL2_mixer>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2IMIXER_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2NET_LIBRARY})

#src/Personajes/[a-z/]*((.hpp)|(.cpp))