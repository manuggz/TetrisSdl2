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

static const int TETRIS_PLAYER_1 = 1;

static const int N_DIGITOS_ENTEROS = 10;
static const char *const META_DATA_HIGHSCORE = "HighScore";
static const int SINGLE_LINE = 1;
static const int DOUBLE_LINE = 2;
static const int TRIPLE_LINE = 3;
static const int TETRIS = 4;


class TetrisInterfaz : public InterfazGrafica, public InterfazJuegoTetris {
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

        mMetaData = new MetaData();
        if(mMetaData->cargarMetaData("resources/settings.db",":")){
            mHighScore = std::stoi(mMetaData->getMetaData(META_DATA_HIGHSCORE));
        }else{
            mHighScore = 0;
        }

//        mMusicaFondo = cargar_musica((char *)"resources/music/music.mp3");
//        mSfxChunkGameOver = cargar_sonido((char *) "resources/music/SFX_GameOver.ogg");
//        mSfxChunkGameStart = cargar_sonido((char *) "resources/music/SFX_GameStart.ogg");
//        mSfxLevelUp =  cargar_sonido((char *) "resources/music/SFX_LevelUp.ogg");
//        mSfxClearLines[0] = cargar_sonido((char *) "resources/music/SFX_SpecialLineClearSingle.ogg");
//        mSfxClearLines[1] = cargar_sonido((char *) "resources/music/SFX_SpecialLineClearDouble.ogg");
//        mSfxClearLines[2] = cargar_sonido((char *) "resources/music/SFX_SpecialLineClearTriple.ogg");
//        mSfxClearLines[3] = cargar_sonido((char *) "resources/music/SFX_SpecialTetris.ogg");


//        mSfxBTBTetris = cargar_sonido((char *) "resources/music/VO_B2BTETRS.ogg");

//        mSfxCongratulate[0] = cargar_sonido((char *) "resources/music/VO_WOW.ogg");;
//        mSfxCongratulate[1] = cargar_sonido((char *) "resources/music/VO_WONDRFL.ogg");;
//        mSfxCongratulate[2] = cargar_sonido((char *) "resources/music/VO_VRYGOOD.ogg");;
//        mSfxCongratulate[3] = cargar_sonido((char *) "resources/music/VO_THTGREAT.ogg");;
//        mSfxCongratulate[4] = cargar_sonido((char *) "resources/music/VO_FANTSTC.ogg");;
//        mSfxCongratulate[5] = cargar_sonido((char *) "resources/music/VO_EXLNT.ogg");;
//        mSfxCongratulate[6] = cargar_sonido((char *) "resources/music/VO_BRILLIANT.ogg");;
//        mSfxCongratulate[7] = cargar_sonido((char *) "resources/music/VO_AMAZING.ogg");;

    }

    void playSfx(Mix_Chunk *pSfxChunk)  {
//        mGameManagerInterfaz->play(pSfxChunk);
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
//                    mGameManagerInterfaz->play(mSfxBTBTetris);
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
//                    mGameManagerInterfaz->play(mSfxCongratulate[rand()%8]);
                }
            }
        }
        mLineasCompletasAnteriores = nLineasCompletadas;


        if(mPuntajePlayer > mHighScore && mHighScore > 0&&!mCongratuledScoreGreaterThanHighScore){
            mCongratuledScoreGreaterThanHighScore = true;
//            mGameManagerInterfaz->play(mSfxCongratulate[rand()%8]);
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
//            mGameManagerInterfaz->play(mSfxLevelUp);
        }

        setTextWithDigits(mBitmapLevelPlayer1Valor,mLineasCompletas,5);

//        mGameManagerInterfaz->play(mSfxClearLines[nLineasCompletadas - 1]);
    }

    void start() override {
        std::cout << "TetrisInterfaz::start" << std::endl;
        InterfazGrafica::start();
        mTetrisJuego->start();
        mControlTimer.start();
//        mGameManagerInterfaz->playFadeInSound(mMusicaFondo,MIX_MAX_VOLUME/2);
//        mGameManagerInterfaz->play(mSfxChunkGameStart);
    }

    void tetrisPaused(int tetrisID) override {
        mControlTimer.pause();
    }

    void tetrisResumed(int tetrisID) override {
        mControlTimer.resume();
    }

    virtual void createUI(SDL_Renderer *renderer)  override{

        std::cout << "TetrisInterfaz::createUI" << std::endl;
        // Establecemos el fondo con los parametros adecuados para que ocupe toda la pantalla
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_HEIGHT,LAYOUT_PARAM_TRUE);
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_FILL_PARENT_WIDTH,LAYOUT_PARAM_TRUE);
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_WRAP_WIDTH,LAYOUT_PARAM_FALSE);
        mLayoutBackGround->setLayoutParam(LAYOUT_PARAM_WRAP_HEIGHT,LAYOUT_PARAM_FALSE);


        mNombreJugador = "Iranid<3";

        //mLayoutBackGround->setBackgroundColor(SDL_Color {27,63,177,255});
        LTexture * lTexture = new LTexture();
        lTexture->cargarDesdeArchivo("resources/backgroundSinglePlayer.png",renderer,false);
        mLayoutBackGround->setBackgroundTexture(lTexture);

        mBitmapFont[NORMAL] = new BitmapFont(renderer,"resources/fuentes/fuente_1.png");
        mBitmapFont[RESALTADO] = new BitmapFont(renderer,"resources/fuentes/fuente_2.png");

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

        mTetrisJuego->crearUI(renderer);
        SDL_ShowCursor(SDL_DISABLE);//ocultamos el cursor

    }

    void setTextWithDigits(BitmapFontRenderer * bitmapFontRenderer,int valor,int nDigitos){
/*        char textoDigitalizado[N_DIGITOS_ENTEROS + 1];

        sprintf(textoDigitalizado,"%*d",nDigitos,valor);

        int i = 0;
        while(textoDigitalizado[i] == ' ')textoDigitalizado[i++]='0';
        bitmapFontRenderer->setText(textoDigitalizado);
*/
    }

    void tetrisGameOver(int tetrisID) override {
//        mGameManagerInterfaz->play(mSfxChunkGameOver);
    }
    void tetrisRetry(int tetrisID) override {
        std::cout << "TetrisInterfaz::tetrisRetry(" << tetrisID << ")" << std::endl;
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
//        mGameManagerInterfaz->play(mSfxChunkGameStart);
    }

    void tetrisEnd(int tetrisID) override {
        actualizarHighScore();
        mGameManagerInterfaz->goBack();
    }

    bool actualizarHighScore(){
        if(mPuntajePlayer > mHighScore){
            mHighScore = mPuntajePlayer;
            mMetaData->setMetaData(META_DATA_HIGHSCORE,std::to_string(mHighScore));
            //mMetaData->guardar("resources/settings.db",":");
            return true;
        }

        return false;

    }
    void nuevoTetrominoSiguiente(Tetromino *nuevoTetrominoSiguiente) override {
        mTetrominoSiguiente = nuevoTetrominoSiguiente;
        mTetrominoSiguiente->move(803,118);
    }

    virtual void resume() override {
        std::cout << "TetrisInterfaz::resume" << std::endl;
        InterfazGrafica::resume();
        mLayoutBackGround->setDisabled(true);
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
                    actualizarHighScore();
                    mGameManagerInterfaz->goBack();
                    //mGameManager->showPopUp()
                    break;
                default:
                    break;
            }
        }

        mTetrisJuego->procesarEvento(evento);
    }

    void stop() override {
        InterfazGrafica::stop();
        //mMetaData->guardar("resources/settings.db",":");
    }

    void update() override {
        InterfazGrafica::update();
        mTetrisJuego->update();
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
        delete mMetaData;

//        Mix_FreeMusic(mMusicaFondo);
//        Mix_FreeChunk(mSfxChunkGameOver);
//        Mix_FreeChunk(mSfxChunkGameStart);
//        Mix_FreeChunk(mSfxLevelUp);
//        Mix_FreeChunk(mSfxBTBTetris);

        for(int i = 0 ; i < 8 ; i++) {
//            Mix_FreeChunk(mSfxCongratulate[i]);
        }

        for(int i = 0; i < 4;i++){
//            Mix_FreeChunk(mSfxClearLines[i]);
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
    MetaData *mMetaData = nullptr;
    int mHighScore;

//    Mix_Music * mMusicaFondo = nullptr;

//    Mix_Chunk * mSfxChunkGameStart = nullptr;
//    Mix_Chunk * mSfxChunkGameOver = nullptr;
//    Mix_Chunk * mSfxLevelUp = nullptr;

//    Mix_Chunk * mSfxBTBTetris = nullptr;

//    Mix_Chunk * mSfxCongratulate[8] {nullptr};

//    Mix_Chunk * mSfxClearLines[4] {nullptr};

    int mLevelTetrisPlayer = 1;
    int mLineasCompletasAnteriores = 0;
    bool mCongratuledScoreGreaterThanHighScore = false;
    std::string mNombreJugador;
};

#endif //TETRIS_TETRIS_HPP
