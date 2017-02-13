//
// Created by manuggz on 06/12/16.
//

#ifndef TETRIS_TETRIS_HPP
#define TETRIS_TETRIS_HPP

#include "TetrisJuego.hpp"
#include "engine/interfaces/InterfazGrafica.hpp"
#include "engine/layout/LayoutManager/LayoutAbsolute.hpp"
#include "engine/util/CMetaData.hpp"
#include "engine/util/BitmapFont.hpp"
#include "engine/util/LTimer.hpp"
#include "ConfiguracionSingleton.hpp"
#include "engine/util/MusicaFondo.hpp"
#include "engine/util/EfectoSonido.hpp"
#include "MenuContextual.hpp"
#include "MenuContextualAnimado.hpp"
#include "PopUpOpciones.hpp"

static const int TETRIS_PLAYER_1 = 1;

static const int N_DIGITOS_ENTEROS = 10;
static const int SINGLE_LINE = 1;
static const int DOUBLE_LINE = 2;
static const int TRIPLE_LINE = 3;
static const int TETRIS = 4;


static const int ID_OPCION_MENU_CONTINUE = 1;

static const int ID_OPCION_MENU_RETRY = 2;

static const int ID_OPCION_MENU_OPTIONS = 3;

static const int ID_OPCION_MENU_END = 4;

class TetrisInterfaz : public InterfazGrafica, public InterfazJuegoTetris, public ContextMenuParent {
public:


    TetrisInterfaz(GameManagerInterfazUI *gameManagerInterfaz)
            : InterfazGrafica(gameManagerInterfaz) {
        std::cout << "TetrisInterfaz::TetrisInterfaz" << std::endl;

    }

    void prepare() override {
        std::cout << "TetrisInterfaz::prepare" << std::endl;
        InterfazGrafica::prepare();
        mLayoutBackGround          = new LayoutAbsolute();
        mTetrisJuego = new TetrisJuego(this, TETRIS_PLAYER_1, 288 + 32, 0);

        configuracionSingleton = ConfiguracionSingletonManager::obtenerConfiguracion();
        mHighScore = configuracionSingleton->obtenerEntero(ID_HIGHSCORE,0);

        mMusicaFondo = new MusicaFondo("resources/music/music.mp3");

        mSfxChunkGameOver = new EfectoSonido("resources/music/SFX_GameOver.ogg",MIX_MAX_VOLUME);
        mSfxChunkGameStart = new EfectoSonido("resources/music/SFX_GameStart.ogg",MIX_MAX_VOLUME);
        mSfxLevelUp =  new EfectoSonido("resources/music/SFX_LevelUp.ogg",MIX_MAX_VOLUME);
        mSfxClearLines[0] = new EfectoSonido("resources/music/SFX_SpecialLineClearSingle.ogg",MIX_MAX_VOLUME);
        mSfxClearLines[1] = new EfectoSonido("resources/music/SFX_SpecialLineClearDouble.ogg",MIX_MAX_VOLUME);
        mSfxClearLines[2] = new EfectoSonido("resources/music/SFX_SpecialLineClearTriple.ogg",MIX_MAX_VOLUME);
        mSfxClearLines[3] = new EfectoSonido("resources/music/SFX_SpecialTetris.ogg",MIX_MAX_VOLUME);


        mSfxBTBTetris = new EfectoSonido("resources/music/VO_B2BTETRS.ogg",MIX_MAX_VOLUME);

        mSfxCongratulate[0] = new EfectoSonido("resources/music/VO_WOW.ogg",MIX_MAX_VOLUME);;
        mSfxCongratulate[1] = new EfectoSonido("resources/music/VO_WONDRFL.ogg",MIX_MAX_VOLUME);;
        mSfxCongratulate[2] = new EfectoSonido("resources/music/VO_VRYGOOD.ogg",MIX_MAX_VOLUME);;
        mSfxCongratulate[3] = new EfectoSonido("resources/music/VO_THTGREAT.ogg",MIX_MAX_VOLUME);;
        mSfxCongratulate[4] = new EfectoSonido("resources/music/VO_FANTSTC.ogg",MIX_MAX_VOLUME);;
        mSfxCongratulate[5] = new EfectoSonido("resources/music/VO_EXLNT.ogg",MIX_MAX_VOLUME);;
        mSfxCongratulate[6] = new EfectoSonido("resources/music/VO_BRILLIANT.ogg",MIX_MAX_VOLUME);;
        mSfxCongratulate[7] = new EfectoSonido("resources/music/VO_AMAZING.ogg",MIX_MAX_VOLUME);;

    }

    void tetrisHardDrop(int tetrisID, int nCells)override {
        if(nCells > 40) nCells = 40;
        mPuntajePlayer +=  2*nCells;
        setTextWithDigits(mBitmapScorePlayer1Valor,mPuntajePlayer,N_DIGITOS_ENTEROS);
    }

    void tetrisSoftDrop(int tetrisID, int nCells) override{
        if(nCells > 20) nCells = 20;
        mPuntajePlayer +=  nCells;
        setTextWithDigits(mBitmapScorePlayer1Valor,mPuntajePlayer,N_DIGITOS_ENTEROS);
    }

    void tetrisLineasCompletadas(int TetrisID, int nLineasCompletadas) override {

        switch(nLineasCompletadas){
            case SINGLE_LINE:
                mPuntajePlayer += 100*mLevelTetrisPlayer;
                break;
            case DOUBLE_LINE:
                mPuntajePlayer += 300*mLevelTetrisPlayer;
                break;
            case TRIPLE_LINE:
                mPuntajePlayer += 500*mLevelTetrisPlayer;
                break;
            case TETRIS:
                if(mLineasCompletasAnteriores == TETRIS){
                    mPuntajePlayer += 1200 * mLevelTetrisPlayer;
                    if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxBTBTetris->play();
                }else{
                    mPuntajePlayer += 800*mLevelTetrisPlayer;
                }
                break;
            default:break; // Help to avoid warnings!
        }

        if(mLineasCompletasAnteriores){
            mPuntajePlayer += 50*mLineasCompletasAnteriores*mLevelTetrisPlayer;

            if(mLineasCompletas >= DOUBLE_LINE && mLineasCompletasAnteriores >= DOUBLE_LINE){
                if(rand()%3 == 1){
                    if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxCongratulate[rand()%8]->play();
                }
            }
        }
        mLineasCompletasAnteriores = nLineasCompletadas;


        if(mPuntajePlayer > mHighScore && mHighScore > 0&&!mCongratuledScoreGreaterThanHighScore){
            mCongratuledScoreGreaterThanHighScore = true;
            if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxCongratulate[rand()%8]->play();
            mBitmapScorePlayer1Valor->setBitmapFont(mBitmapFont[RESALTADO]);
        }

        setTextWithDigits(mBitmapScorePlayer1Valor,mPuntajePlayer,10);

        mLineasCompletas += nLineasCompletadas;
        if((mLineasCompletas / 10 + 1) > mLevelTetrisPlayer){
            int nuevoTick = mTetrisJuego->getTickDelayBajarTetromino() - 2;
            if(nuevoTick <= 2){
                nuevoTick = 2;
            }
            mTetrisJuego->setTickDelayBajarTetromino(nuevoTick);
            mLevelTetrisPlayer += 1;
            setTextWithDigits(mBitmapLevelPlayer1Valor,mLevelTetrisPlayer,3);
            if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxLevelUp->play();
        }

        setTextWithDigits(mBitmapLevelPlayer1Valor,mLineasCompletas,5);

        if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxClearLines[nLineasCompletadas - 1]->play();
    }

    void start() override {
        std::cout << "TetrisInterfaz::start" << std::endl;
        InterfazGrafica::start();
        mTetrisJuego->start();
        mControlTimer.start();
        if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxChunkGameStart->play();
    }

    void tetrisPresionadoPause(int tetrisID) override {
        mControlTimer.pause();
        menuPausa->setVisible(true);
        mTetrisJuego->pause();
        menuPausa->establecerResaltado(ID_OPCION_MENU_CONTINUE);
    }

    virtual void createUI(SDL_Renderer *renderer)  override{

        std::cout << "TetrisInterfaz::createUI" << std::endl;
        // Establecemos el fondo con los parametros adecuados para que ocupe toda la pantalla
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_HEIGHT,LAYOUT_PARAM_TRUE);
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_WIDTH,LAYOUT_PARAM_TRUE);
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_WRAP_WIDTH,LAYOUT_PARAM_FALSE);
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_WRAP_HEIGHT,LAYOUT_PARAM_FALSE);


        mNombreJugador = configuracionSingleton->obtenerString(ID_NICK_NAME,DEFAULT_TEXT_NICK);

        //mLayoutBackGround->setBackgroundColor(SDL_Color {27,63,177,255});
        LTexture * lTexture = new LTexture();
        lTexture->cargarDesdeArchivo("resources/images/backgroundSinglePlayer.png",renderer,false);
        mLayoutBackGround->setBackgroundTexture(lTexture);

        mBitmapFont[NORMAL] = new BitmapFont(renderer,"resources/fuentes/tetris_fuente_1_32_blanca.png");
        mBitmapFont[RESALTADO] = new BitmapFont(renderer,"resources/fuentes/tetris_fuente_1_32_blanca.png");

        mBitmapHighScorePlayer1Valor = new BitmapFontRenderer(mBitmapFont[NORMAL],30,217);
        setTextWithDigits(mBitmapHighScorePlayer1Valor,mHighScore,N_DIGITOS_ENTEROS);

        mNombreJugadorPlayer1Valor = new BitmapFontRenderer(mBitmapFont[NORMAL],42,105);
        mNombreJugadorPlayer1Valor->setText(mNombreJugador);

        mBitmapScorePlayer1Valor = new BitmapFontRenderer(mBitmapFont[NORMAL],30,485);
        mBitmapScorePlayer1Valor->setText("0000000000");

		mBitmapTimePlayer1Valor = new BitmapFontRenderer(mBitmapFont[NORMAL],0,578);
        mBitmapTimePlayer1Valor->setRight(270);
        mBitmapTimePlayer1Valor->setText("00:00:00");

        
        mBitmapLevelPlayer1Valor = new BitmapFontRenderer(mBitmapFont[NORMAL],0,642);
        mBitmapLevelPlayer1Valor->setRight(270);
        mBitmapLevelPlayer1Valor->setText("001");

        mBitmapLinesPlayer1Valor = new BitmapFontRenderer(mBitmapFont[NORMAL],627,706);
        mBitmapLinesPlayer1Valor->setRight(270);
        mBitmapLinesPlayer1Valor->setText("00000");

        // Recursos para el menu
        menuPausa = new MenuContextual(this,310,270);

        menuPausa->agregarOpcionMenu("Continue", ID_OPCION_MENU_CONTINUE);
        menuPausa->agregarOpcionMenu("Retry", ID_OPCION_MENU_RETRY);
        menuPausa->agregarOpcionMenu("Options", ID_OPCION_MENU_OPTIONS);
        menuPausa->agregarOpcionMenu("End", ID_OPCION_MENU_END);

        menuPausa->establecerFuente(new BitmapFont(renderer,"resources/fuentes/tetris_fuente_1_32_blanca.png"),
                               MenuContextual::FUENTE_NORMAL);
        menuPausa->establecerFuente(new BitmapFont(renderer,"resources/fuentes/tetris_fuente_1_32_amarilla.png"),
                               MenuContextual::FUENTE_RESALTADA);


        LTexture *textureFlechaDerecha = new LTexture();
        textureFlechaDerecha->cargarDesdeArchivo("resources/images/tetris_flecha_opcion_seleccionada_dir_derecha.png",
                                                 renderer, false);
        menuPausa->establecerTexturaFlechaDerecha(textureFlechaDerecha);

        LTexture *textureFlechaIzquierda = new LTexture();
        textureFlechaIzquierda->cargarDesdeArchivo("resources/images/tetris_flecha_opcion_seleccionada_dir_izquierda.png",
                                                   renderer, false);
        menuPausa->establecerTexturaFlechaIzquierda(textureFlechaIzquierda);

        menuPausa->createUI(renderer);
        LTexture * textureFondoMenu = new LTexture();
        textureFondoMenu->cargarDesdeArchivo("resources/images/menuopciones_placeholder.png",renderer,false);
        menuPausa->establecerTexturaFondo(textureFondoMenu);
        menuPausa->establecerResaltado(ID_OPCION_MENU_CONTINUE);
        menuPausa->setVisible(false);

        mTetrisJuego->crearUI(renderer);
        SDL_ShowCursor(SDL_DISABLE);//ocultamos el cursor

    }

    void setTextWithDigits(BitmapFontRenderer * bitmapFontRenderer,int valor,int nDigitos){
        char textoDigitalizado[N_DIGITOS_ENTEROS + 1];

        sprintf(textoDigitalizado,"%*d",nDigitos,valor);

        int i = 0;
        while(textoDigitalizado[i] == ' ')textoDigitalizado[i++]='0';
        bitmapFontRenderer->setText(textoDigitalizado);

    }

    void tetrisGameOver(int tetrisID) override {
        if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxChunkGameOver->play();
        menuPausa->establecerVisilidad(ID_OPCION_MENU_CONTINUE,false);
        menuPausa->establecerResaltado(ID_OPCION_MENU_RETRY);
    }

    std::string obtenerPrefPath() override {
        return mGameManagerInterfaz->obtenerPrefPath();
    }

    int getJoysActivos() override {
        return mGameManagerInterfaz->getActiveJoys();
    }

    SDL_Joystick *getJoy(int device_index) override {
        return mGameManagerInterfaz->getJoy(device_index);
    }


    bool actualizarHighScore(){
        if(mPuntajePlayer > mHighScore){
            mHighScore = mPuntajePlayer;
            configuracionSingleton->guardarEntero(ID_HIGHSCORE,mHighScore);
            return true;
        }

        return false;

    }
    void nuevoTetrominoSiguiente(Tetromino *nuevoTetrominoSiguiente) override {
        mTetrominoSiguiente = nuevoTetrominoSiguiente;
        mTetrominoSiguiente->move(803,118);
    }

    void resume() override {
        std::cout << "TetrisInterfaz::resume" << std::endl;
        InterfazGrafica::resume();
        mLayoutBackGround->setDisabled(true);
        if(configuracionSingleton->obtenerBooleano(ID_IS_MUSIC_ON,true)){
            mMusicaFondo->play();
        }

        mNombreJugador = configuracionSingleton->obtenerString(ID_NICK_NAME,DEFAULT_TEXT_NICK);
        mNombreJugadorPlayer1Valor->setText(mNombreJugador);

        //SDL_ShowCursor(SDL_DISABLE);//ocultamos el cursor
    }


/**
         * Procesa el evento del usuario
         * Se encarga de mover la opcion resaltada al usuario o en caso de que sea ENTER llamar a la funcion enlazada a
         * la opcion.
         * @param evento  Evento producido por SDL
         */
    void procesarEvento(SDL_Event *evento) override {
        if(evento->type==SDL_KEYDOWN) {
            switch (evento->key.keysym.sym) {
                case SDLK_ESCAPE:
                    if(menuPausa->esVisible()) {
                        if(menuPausa->esOpcionMenuVisible(ID_OPCION_MENU_CONTINUE)) {
                            if (menuPausa->obtenerIDResaltadoActual() != ID_OPCION_MENU_CONTINUE) {
                                menuPausa->establecerResaltado(ID_OPCION_MENU_CONTINUE);
                            }else{
                                opcionSeleccionadaMenuContextual(ID_OPCION_MENU_CONTINUE);
                            }
                        }else{
                            actualizarHighScore();
                            mGameManagerInterfaz->goBack();
                        }
                        return;
                    }else{
                        mControlTimer.pause();
                        menuPausa->setVisible(true);
                        mTetrisJuego->pause();
                        return;
                    }
                    break;
                default:
                    break;
            }
        }

        mTetrisJuego->procesarEvento(evento);
        if(menuPausa->esVisible()){
            menuPausa->procesarEvento(evento);
        }
    }

    void stop() override {
        InterfazGrafica::stop();
    }

    void update() override {
        InterfazGrafica::update();
        mTetrisJuego->update();
        if(menuPausa->esVisible()){
            menuPausa->update();
        }
    }

    void opcionSeleccionadaMenuContextual(int idOpcionSeleccionada) override {
        switch (idOpcionSeleccionada){
            case ID_OPCION_MENU_CONTINUE:
//                            std::cout << "TetrisJuego::CONTINUE" << std::endl;
                //mEstadoJuego = EstadoJuego ::RUNNING;
                mTetrisJuego->resume();
//                            if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxHoldPiece->play();
                menuPausa->setVisible(false);
                mControlTimer.resume();

                break;
            case ID_OPCION_MENU_RETRY:
                menuPausa->establecerVisilidad(ID_OPCION_MENU_CONTINUE,true);
                mTetrisJuego->reset();
                if(actualizarHighScore()){
                    setTextWithDigits(mBitmapHighScorePlayer1Valor,mHighScore,N_DIGITOS_ENTEROS);
                }
                mPuntajePlayer = 0;
                mLineasCompletas = 0;
                mLineasCompletasAnteriores = 0;
                mLevelTetrisPlayer = 1;
                mCongratuledScoreGreaterThanHighScore = false;
                mBitmapScorePlayer1Valor->setBitmapFont(mBitmapFont[NORMAL]);
                mBitmapLinesPlayer1Valor->setText("00000");
                mBitmapLevelPlayer1Valor->setText("001");
                mBitmapScorePlayer1Valor->setText("0000000000");

                //mControlTimer.stop();
                mControlTimer.start();
                if(configuracionSingleton->obtenerBooleano(ID_IS_SFX_ON,true))mSfxChunkGameStart->play();
                menuPausa->setVisible(false);
                break;
            case ID_OPCION_MENU_END:
                actualizarHighScore();
                mGameManagerInterfaz->goBack();
                break;
            case ID_OPCION_MENU_OPTIONS:{
                    PopUpOpciones * popUpOpciones = new PopUpOpciones(mGameManagerInterfaz,ID_POP_UP_OPCIONES);
                    mGameManagerInterfaz->showPopUp(popUpOpciones);
            }
            break;

        }
    }

    void draw(SDL_Renderer *renderer) override {
        if(mLayoutBackGround->isDisabled()){
            SDL_Rect rect = mGameManagerInterfaz->getRectScreen();
            mLayoutBackGround->pack(renderer);
            mLayoutBackGround->setRectDibujo(rect);
        }
        mLayoutBackGround->draw(renderer);
        mBitmapHighScorePlayer1Valor->draw(renderer);
        mBitmapScorePlayer1Valor->draw(renderer);
        mNombreJugadorPlayer1Valor->draw(renderer);

        if(mControlTimer.isRunning()){
            static char hrs[3],min_[3],seg[3],tiempo[6];

            int segundos = mControlTimer.getTicks()/1000;

            sprintf(seg,"%2d",(segundos) % 60);
            if(seg[0]==' ')seg[0]='0';

            int minutos = (segundos - segundos%60)/60;
            sprintf(min_,"%2d",minutos%60);
            if(min_[0]==' ')min_[0]='0';

            int horas = (minutos - minutos%60)/60;
            sprintf(hrs,"%2d",horas);
            if(hrs[0]==' ')hrs[0]='0';

            sprintf(tiempo,"%s:%s:%s",hrs,min_,seg);
            mBitmapTimePlayer1Valor->setText(tiempo);

        }
        mBitmapTimePlayer1Valor->draw(renderer);
        mBitmapLevelPlayer1Valor->draw(renderer);
        mBitmapLinesPlayer1Valor->draw(renderer);

        mTetrisJuego->draw(renderer);

        if(menuPausa->esVisible()){
            menuPausa->draw(renderer);
        }
        if(mTetrominoSiguiente != nullptr) mTetrominoSiguiente->draw(renderer);
    }

    ~TetrisInterfaz(){
        std::cout << "~TetrisInterfaz()" << std::endl;

        delete mLayoutBackGround; // Al liberar el layout parent se liberan todos sus mComponentes
        delete mTetrisJuego;
        //delete mLabelComponentScoreActual;
        delete mBitmapHighScorePlayer1Valor;
        delete mBitmapScorePlayer1Valor;
        delete mBitmapTimePlayer1Valor;
        delete mBitmapLevelPlayer1Valor;
        delete mBitmapLinesPlayer1Valor;
        delete mNombreJugadorPlayer1Valor;

        delete mMusicaFondo;
        delete mSfxChunkGameOver;
        delete mSfxChunkGameStart;
        delete mSfxLevelUp;
        delete mSfxBTBTetris;

        delete menuPausa;
        for(int i = 0 ; i < 8 ; i++) {
            delete mSfxCongratulate[i];
        }

        for(int i = 0; i < 4;i++){
            delete mSfxClearLines[i];
        }

        for(int i = 0;i < 2;i++){
            delete mBitmapFont[i];
        }
    }

private:

    LayoutAbsolute *mLayoutBackGround = nullptr;

    TetrisJuego * mTetrisJuego = nullptr;
    BitmapFontRenderer *mBitmapHighScorePlayer1Valor = nullptr;
    BitmapFontRenderer *mBitmapScorePlayer1Valor = nullptr;
    BitmapFontRenderer *mBitmapTimePlayer1Valor = nullptr;
    BitmapFontRenderer *mBitmapLevelPlayer1Valor = nullptr;
    BitmapFontRenderer *mBitmapLinesPlayer1Valor = nullptr;
    BitmapFontRenderer *mNombreJugadorPlayer1Valor = nullptr;

    enum EstadoTexto{
        NORMAL,
        RESALTADO
    };
    BitmapFont *mBitmapFont[2];
    Tetromino * mTetrominoSiguiente = nullptr;
    LTimer mControlTimer;

    int mPuntajePlayer = 0;
    int mLineasCompletas = 0;
    //MetaData *mMetaData = nullptr;
    ConfiguracionSingleton * configuracionSingleton;
    int mHighScore;


    EfectoSonido * mSfxChunkGameStart = nullptr;
    EfectoSonido * mSfxChunkGameOver = nullptr;
    EfectoSonido * mSfxLevelUp = nullptr;

    EfectoSonido * mSfxBTBTetris = nullptr;

    EfectoSonido * mSfxCongratulate[8] {nullptr};

    EfectoSonido * mSfxClearLines[4] {nullptr};

    int mLevelTetrisPlayer = 1;
    int mLineasCompletasAnteriores = 0;
    bool mCongratuledScoreGreaterThanHighScore = false;
    std::string mNombreJugador;
    MusicaFondo *mMusicaFondo;
    MenuContextual *menuPausa;
};

#endif //TETRIS_TETRIS_HPP
