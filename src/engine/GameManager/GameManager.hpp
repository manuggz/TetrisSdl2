#ifndef ENGINE_GAME_MANAGER_HPP
#define ENGINE_GAME_MANAGER_HPP

#include <ctime>
#include <deque>
#include <stack>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>

#include "../interfaces/InterfazGrafica.hpp"
#include "../interfaces/PopUpInterfaz.hpp"

#include "../util/LTimer.hpp"
#include "../util/util.hpp"
#include "../util/Toast.hpp"

class InterfazEstandarBackResult{
public:
    std::string texto = "";
    int entero = 0;
    ~InterfazEstandarBackResult(){

    }
};

/**
 *
 * <h1> Administra el Juego</h1>
 *
 * Se encarga de manejar la interacción entre las llamadas a SDL y las peticiones de las interfaces.
 *
 * Una Interfaz(@link InterazUI) es aquella que interactua con el usuario así mismo lo hace un Pop Up.

 * Esta clase se encarga:
 *  <ul>
 *      <li> Interacciones entre Interfaces</li>
 *      <li> Interaccion entre Interfaz y PopUp</li>
 *      <li> Interacciones entre Interfaz/PopUp y llamadas a SDL(Sonido,Joystick,Eventos)</li>
 *      <li> Interacciones entre Eventos del Usuario(Salir) e Interfaces</li>
 *  </ul>
 *
 *  Deriva de {@link GameManagerInterfazUI} que basicamente es un Contrato entre la interfaz y el Game Manager.
 *  Un conjunto de funciones que esta clase debe implementar para que una interfaz pueda referenciarla.
 *  Ya que una Interfaz referencia una instancia de {@link GameManagerInterfazUI}.
 *  (Esto es para evitar problemas de doble referenciacion)

 *  @author Manuel González <manuelggonzalezm@gmail.com>
 *
 *
 */
class GameManager : public GameManagerInterfazUI{

public:


    GameManager(std::string nombreApp,std::string ruta_icono, unsigned int width,unsigned int height,bool pantallaCompleta);

    SDL_Joystick * getJoy(int device_index) override ;
    int getActiveJoys() ;
    void cambiarInterfaz(InterfazGrafica * nueva) ;

    void run();
    void quit(){salir_juego=true;};

    int getWidth();
    int getHeight();

    ~GameManager();
    void goBack() ;

    void setRoot(InterfazGrafica *nuevaInterfazRoot) ;

    SDL_Rect getRectScreen() ;

    void closePopUp(InterfazEstandarBackResult * result = nullptr) ;

    void showPopUp(PopUpInterfaz *pPopUp,int showPopUp) ;
    // Volumen 0-128
    void playSound(Mix_Music *music,Uint8 volumen) ;

    void play(Mix_Chunk *pSfxChunk) ;
    void playFadeInSound(Mix_Music *music, Uint8 volumen) ;

    Uint32 getWindowPixelFormat() override;

    void cambiarInterfaz(InterfazGrafica *pInterfaz, int ID) override;

    void goBack(InterfazEstandarBackResult *pResult) override;

    int getNativeWidth() override;

    int getNativeHeight() override;

    float getScaleRatioW() const;

    void setScaleRatioW(float scaleRatioW);

    float getScaleRatioH() const;

    void setScaleRatioH(float scaleRatioH);

    const std::string &getNombreOrganization() const;

    void setNombreOrganization(const std::string &nombreOrganization);

    const std::string &getNombreApp() const;

    void setNombreApp(const std::string &nombreApp);
    std::string obtenerPrefPath() override;

    void setPuedeToglearPantallaCompleta(bool puedeToglear);

    void mostrarToast(Toast * toast);
private:

    int mFPS = 60;
    int mScreenTicksPerFrame = 1000 / mFPS;

    std::stack<InterfazGrafica *> interfaces; // Pila de interfaces
    InterfazGrafica * interfaz_actual = nullptr;
    SDL_Window *mMainWindow = nullptr;
    SDL_Renderer *gRenderer = nullptr;

    SDL_Joystick ** pJoystick;

    bool mIniciadoModuloSonido=false;
    bool salir_juego = false;

    int mWidth = 0;
    int mHeight = 0;

    PopUpInterfaz * mpPopUp = nullptr;
    InterfazEstandarBackResult *mpResultPopUp = nullptr;
    // IDentificador del Actual PopUp
    // USado para identificar en el codigo cual es el popup que se cerró
    int mIDCodePopUp;
    std::string mRutaIcono;

    int mChannels = 5;
    int mJoysticksActivos = 5;


    bool mIsPantallaCompleta = false;


    SDL_Rect nativeSize;
    SDL_Rect newWindowSize;
    float scaleRatioW;
    float scaleRatioH;

    SDL_Texture * mpTextureBufferTarget;

    bool resize;

    void iniciarLibreriaSDL();
    void establecerModoDeVideo(bool pantalla_completa = false);
    void activarJoysticks();
    bool procesarEventos();

    void Resize();

    bool mISFullScreenPressed = false;

    std::string rutaPathPrefPath;
    std::string nombreOrganization;
    std::string nombreApp;

    int idInterfazActual;

    bool puedeToglearPantallaCompleta = true;
    InterfazEstandarBackResult *mpResultInterfazActual = nullptr;

    Toast * mpToastMostrando = nullptr;
};
#endif
