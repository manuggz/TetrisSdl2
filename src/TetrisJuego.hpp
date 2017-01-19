//
// Created by manuggz on 06/12/16.
//

#ifndef TETRIS_TETRISJUEGO_HPP
#define TETRIS_TETRISJUEGO_HPP


#include <SDL2/SDL.h>
#include <SDL_mixer.h>
#include <deque>
#include "Tetromino.hpp"
#include "ControlaAnimacion.hpp"
#include "AnimacionDesvanecerOut.hpp"
#include "RandomGenerator.hpp"
#include "ControlTeclasPlayer.hpp"
#include "engine/util/util.hpp"
#include "engine/util/SpriteSheet.hpp"
#include "engine/util/BitmapFont.hpp"

static const int GRUPO_ANIMACION_ELIMINAR_LINEAS = 0;

static const int POS_NO_VISITADA_RECURSION = 0;
static const int POS_VISITADA_RECURSION = 1;

class InterfazJuegoTetris{
public:
    //virtual void tetrisPuntajeCambio(int TetrisID,int nuevoPuntaje) = 0;

    virtual void nuevoTetrominoSiguiente(Tetromino *nuevoTetrominoSiguiente) = 0;
    virtual void tetrisRetry(int tetrisID)  = 0;
    virtual void tetrisEnd(int tetrisID) = 0;

    virtual void tetrisLineasCompletadas(int tetrisID,int nLineas) = 0;

    virtual void tetrisPaused(int tetrisID) = 0;
    virtual void tetrisResumed(int tetrisID) = 0;
    virtual void tetrisGameOver(int tetrisID) = 0;
//    virtual void playSfx(Mix_Chunk * pSfxChunk) = 0;
    virtual void tetrisHardDrop(int tetrisID,int nCells) = 0;
    virtual void tetrisSoftDrop(int tetrisID,int nCells) = 0;
};

class TetrisJuego : public ControlaAnimacionInterfaz {

public:

    TetrisJuego(InterfazJuegoTetris * parent,int id,int x,int y){

        mParent = parent;
        mId = id;
        mSizeCuadro = 32;
        mFilas      = 23;
        mColumnas   = 10;

        mRectAreaJuego.x = x;
        mRectAreaJuego.y = y;
        mRectAreaJuego.w = mColumnas * mSizeCuadro;
        mRectAreaJuego.h = mFilas    * mSizeCuadro;

		// Esta parte se puede quitar, es solo que antes las filas y las columnas no eran fijas
        mArrayEstadoJuegoTetrominosCaidos = new Uint8[mFilas * mColumnas];
        mArrayEstadoJuegoRecursion		  = new Uint8[mFilas * mColumnas];

//		mSfxPieceRotate = cargar_sonido((char *) "resources/music/SFX_PieceRotateLR.ogg");
//        mSfxPieceRotateFail = cargar_sonido((char *) "resources/music/SFX_PieceRotateFail.ogg");
//        mSfxPieceTouch = cargar_sonido((char *) "resources/music/SFX_PieceTouchLR.ogg");
//        mSfxPieceLockDown = cargar_sonido((char *) "resources/music/SFX_PieceLockdown.ogg");
//        mSfxPieceFall = cargar_sonido((char *) "resources/music/SFX_PieceFall.ogg");
//        mSfxPieceMove = cargar_sonido((char *) "resources/music/SFX_PieceMoveLR.ogg");
//        mSfxSoftDrop = cargar_sonido((char *) "resources/music/SFX_PieceSoftDrop.ogg");
//        mSfxHardDrop = cargar_sonido((char *) "resources/music/SFX_PieceHardDrop.ogg");
//        mSfxHoldPiece = cargar_sonido((char *) "resources/music/SFX_PieceHold.ogg");

        mControlPlayerTeclas = new ControlTeclasPlayer();
        mControlPlayerTeclas->setTeclaRepeatDelay(ControlTeclasPlayer::TECLA_HARD_DROP,ControlTeclasPlayer::NO_REPEAT);
        mControlPlayerTeclas->setTeclaRepeatDelay(ControlTeclasPlayer::TECLA_GIRAR_LEFT,ControlTeclasPlayer::DELAY_REPEAT_MEDIUM);
        mControlPlayerTeclas->setTeclaRepeatDelay(ControlTeclasPlayer::TECLA_GIRAR_RIGHT,ControlTeclasPlayer::DELAY_REPEAT_MEDIUM);
    }

    void crearUI(SDL_Renderer * gRenderer){

		mGRenderer = gRenderer;
		
		mSpriteSheetBloques = new SpriteSheet();
		mSpriteSheetBloques->cargarDesdeArchivo(gRenderer,"resources/blocks.png",5,8,false);

        for(int i = 1;i <= 2;i++){
            mpBitmapFont[i - 1] = new BitmapFont(gRenderer,"resources/fuentes/fuente_" + std::to_string(i) + ".png");
        }

        mpBitFntRendOpsMenuPausa[OpcionesPausa::CONTINUE] = new BitmapFontRenderer(
                mpBitmapFont[EstadoOpcionMenu::NORMAL],418,271);
        mpBitFntRendOpsMenuPausa[OpcionesPausa::CONTINUE]->setText("CONTINUE");

        mpBitFntRendOpsMenuPausa[OpcionesPausa::RETRY] = new BitmapFontRenderer(
                mpBitmapFont[EstadoOpcionMenu::NORMAL],418,313);
        mpBitFntRendOpsMenuPausa[OpcionesPausa::RETRY]->setText("RETRY");

        mpBitFntRendOpsMenuPausa[OpcionesPausa::END] = new BitmapFontRenderer(
                mpBitmapFont[EstadoOpcionMenu::NORMAL],418,354);
        mpBitFntRendOpsMenuPausa[OpcionesPausa::END]->setText("END");

        mOpcionSeleccionadaMenuPausa = 0;

        plTextureFlechaOpcionSeleccionada = new LTexture();
        plTextureFlechaOpcionSeleccionada->cargarDesdeArchivo("resources/flecha_opcion_seleccionada.png",gRenderer,false);

        plTextureGameOver = new LTexture();
        plTextureGameOver->cargarDesdeArchivo("resources/game_over_text.png",gRenderer,false);

    }

    void start(){
        std::cout << "TetrisJuego::start" << std::endl;
        reset();
    }

    void reset() {        
		std::cout << "TetrisJuego::reset" << std::endl;

		std::fill_n(mArrayEstadoJuegoTetrominosCaidos,mColumnas*mFilas,0);

        if(mTetrominoActual){
			eliminarTetrominos();
		}

        if(mControlAnimaciones){
            delete mControlAnimaciones;
            mControlAnimaciones = nullptr;
        }

        mControlAnimaciones = new ControlaAnimacion(this);

		mDelayActualBajarTetrominoActual = 0;
        mMaxDelayBajarTetrominoActual	 = 40;

		mEstadoJuego = EstadoJuego ::RUNNING;
        
		//mMantieneTeclaPausaPresionada = true;
		mTetrominoGhost = generarTetrominoGhost();
		crearTetrominos();
    }
	
	void eliminarTetrominos() {
		delete mTetrominoActual;
		mTetrominoActual = nullptr;

		delete mTetrominoSiguiente;
		mTetrominoSiguiente = nullptr;

		delete mTetrominoGhost;
		mTetrominoGhost = nullptr;
	}
	Tetromino * generarTetrominoGhost() {
		Tetromino * tetrominoGhost = new Tetromino(Tetromino::Z, 0, 0, mSizeCuadro,
			new SpriteSheet(mGRenderer, "resources/blocks.png", 5, 8, false), false);

		tetrominoGhost->setAlpha(150);
		tetrominoGhost->setIndiceCuadroBloque(12);
		return tetrominoGhost;
	}

	Tetromino * generarTetrominoAleatorio(){
        return new Tetromino((Tetromino::TetrisForma)mRandomGenerator.getNextFormaTetromino(),0,0,mSizeCuadro,mSpriteSheetBloques);
    }

    void crearTetrominos(){

        if(mTetrominoSiguiente == nullptr){
            mTetrominoActual    = generarTetrominoAleatorio();
        }else{
            mTetrominoActual    = mTetrominoSiguiente;
        }

        mTetrominoSiguiente = generarTetrominoAleatorio();
        mParent->nuevoTetrominoSiguiente(mTetrominoSiguiente);


        mTetrominoActual->move(mRectAreaJuego.x + (mColumnas/2)*mSizeCuadro,mRectAreaJuego.y);

        if(estaTetraminoEnPosicionInvalida(mTetrominoActual)){
            mEstadoJuego = EstadoJuego ::GAME_OVER;
            mParent->tetrisGameOver(mId);
        }

        mTetrominoGhost->cambiarForma(mTetrominoActual->getForma());
    }

    void estadoRunningDraw(SDL_Renderer * gRenderer){

        dibujarArrayFondoJuego(gRenderer);

        if(mTetrominoActual != nullptr) {
            mTetrominoGhost->draw(gRenderer);
            mTetrominoActual->draw(gRenderer);
        }

        for (auto itBloqueAnimacionCayendo = mDequeTetraminosCayendo.begin();
             itBloqueAnimacionCayendo != mDequeTetraminosCayendo.end();
             itBloqueAnimacionCayendo++) {
            (*itBloqueAnimacionCayendo)->draw(gRenderer);
        }

        mControlAnimaciones->draw(gRenderer);
    }

    void estadoPausadoDraw(SDL_Renderer * gRenderer){

        for(int i = 0; i < N_OPCIONES_PAUSA;i++){
            mpBitFntRendOpsMenuPausa[i]->draw(gRenderer);
        }
        plTextureFlechaOpcionSeleccionada->draw(
                gRenderer,
                mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->getLeft() - plTextureFlechaOpcionSeleccionada->getWidth() - 3,
                mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->getTop());
    }

    void dibujarArrayFondoJuego(SDL_Renderer * gRenderer){
        for(int i = 0 ; i < mFilas ; i++ ){
            for(int j = 0 ; j < mColumnas ; j++){
                if(mArrayEstadoJuegoTetrominosCaidos[i*mColumnas + j] != 0){
                    mSpriteSheetBloques->setCurrentCuadro(mArrayEstadoJuegoTetrominosCaidos[i*mColumnas + j] - 1);
                    mSpriteSheetBloques->draw(gRenderer,j*mSizeCuadro + mRectAreaJuego.x,i*mSizeCuadro + mRectAreaJuego.y);
                }
            }
        }
    }
    void estadoGameOverDraw(SDL_Renderer * gRenderer){

        dibujarArrayFondoJuego(gRenderer);
        plTextureGameOver->draw(gRenderer,335,346);
    }

    void draw(SDL_Renderer *  gRenderer){

        switch(mEstadoJuego){

            case STOPPED:break;
            case RUNNING:
                estadoRunningDraw(gRenderer);
                break;
            case PAUSADO:
                estadoPausadoDraw(gRenderer);
                break;
            case GAME_OVER:
                estadoGameOverDraw(gRenderer);
                break;
        }


    }

    bool moverIpTetromino(Tetromino *pForma, int aum_x, int aum_y) {
        pForma->moveIp(aum_x, aum_y);
        if(estaTetraminoEnPosicionInvalida(pForma)){
            pForma->moveIp(-aum_x, -aum_y);
            return false;
        }

        return true;
    }

    int getFilaFromWorldY(int y){
        return  (y - mRectAreaJuego.y)/mSizeCuadro;
    }
    int getColumnaFromWorldX(int x){
        return  (x - mRectAreaJuego.x)/mSizeCuadro;
    }

    bool estaTetraminoEnPosicionInvalida(Tetromino *pForma){

        static SDL_Rect rectTmp {0,0,mSizeCuadro,mSizeCuadro};
        for(int i = 0; i < pForma->getNFilas(); i++ ){
            for(int j = 0; j < pForma->getNColumnas();j++){
                if(pForma->valorIndiceAC(i, j)!= 0){
                    rectTmp.x = j*mSizeCuadro + pForma->getX();
                    rectTmp.y = i*mSizeCuadro + pForma->getY();
                    if(!rect_contain(mRectAreaJuego, rectTmp)){
                        return true;
                    }

                    if(getValorJuegoArray(i + getFilaFromWorldY(pForma->getY()),
                                          j + getColumnaFromWorldX(pForma->getX()))!= 0){
                        return true;
                    }

                }
            }
        }

        return false;
    }

    void procesarEventoRunning(SDL_Event * evento){
    }
    void procesarEvento(SDL_Event * evento){

        switch(mEstadoJuego){

            case STOPPED:break;
            case RUNNING:
                procesarEventoRunning(evento);
                break;
            case PAUSADO:
                procesarEventoPausado(evento);
                break;
            case GAME_OVER:
                procesarEventoGameOver(evento);
                break;
        }
    }

    void procesarEventoPausado(SDL_Event * evento){

        if(evento->type == SDL_KEYDOWN){
            switch (evento->key.keysym.sym) {
                case SDLK_UP:
                    if(mOpcionSeleccionadaMenuPausa - 1 >= 0){
                        mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[NORMAL]);
                        mOpcionSeleccionadaMenuPausa--;
//                        mParent->playSfx(mSfxPieceTouch);
                        mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[RESALTADO]);
                    }
                    break;
                case SDLK_DOWN:
                    if(mOpcionSeleccionadaMenuPausa + 1 < N_OPCIONES_PAUSA){
                        mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[NORMAL]);
                        mOpcionSeleccionadaMenuPausa++;
//                        mParent->playSfx(mSfxPieceTouch);
                        mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[RESALTADO]);
                    }
                    break;
                case SDLK_RETURN:
                    mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[NORMAL]);
                    switch(mOpcionSeleccionadaMenuPausa){
                        case OpcionesPausa::CONTINUE:
                            std::cout << "TetrisJuego::CONTINUE" << std::endl;
                            mEstadoJuego = EstadoJuego ::RUNNING;
                            mParent->tetrisResumed(mId);
//                            mParent->playSfx(mSfxHoldPiece);
                            break;
                        case OpcionesPausa ::RETRY:
                            std::cout << "TetrisJuego::RETRY" << std::endl;
                            mParent->tetrisRetry(mId);
                            break;
                        case OpcionesPausa ::END:
                            std::cout << "TetrisJuego::END" << std::endl;
                            mParent->tetrisEnd(mId);
                            break;
                        default:break; // No debería ocurrir, ayuda a quitar warnings
                    }
                    mOpcionSeleccionadaMenuPausa = 0;
                    break;
            }
        }


    }

    void procesarEventoGameOver(SDL_Event * evento){
        if(evento->type == SDL_KEYDOWN){

            if(evento->key.keysym.sym == SDLK_RETURN){
                mParent->tetrisRetry(mId);
            }
        }
    }


    bool girarFormaActualDesc(int direccion) {
        if(!mTetrominoActual) return false;

        mTetrominoActual->rotate(direccion);

        if(estaTetraminoEnPosicionInvalida(mTetrominoActual)){

            mTetrominoActual->moveIp(mSizeCuadro,0);

            if(estaTetraminoEnPosicionInvalida(mTetrominoActual)){

                mTetrominoActual->moveIp(-2*mSizeCuadro,0);

                if(estaTetraminoEnPosicionInvalida(mTetrominoActual)){
                    mTetrominoActual->rotate(-direccion);
                    mTetrominoActual->moveIp(mSizeCuadro,0);
//                    mParent->playSfx(mSfxPieceRotateFail);
                    return false;
                }
            }
        }else{
//            mParent->playSfx(mSfxPieceRotate);
        }
        return true;
    }

    int hardDropTetramino(Tetromino *pForma) {

        int celdasAvanzadas = 0;
        while(moverIpTetromino(pForma, 0, mSizeCuadro)){
            celdasAvanzadas++;
        };

        return celdasAvanzadas;
    }

    int limpiarLineasCompletas() {

        int nLineasEliminadas = 0;
        bool estaLineaCompleta;
        for(int i = 0 ; i < mFilas; i++){

            estaLineaCompleta = true;
            for(int j = 0; j < mColumnas ;j++){
                if(mArrayEstadoJuegoTetrominosCaidos[i*mColumnas + j] == 0){
                    estaLineaCompleta = false;
                    break;
                }
            }

            if(estaLineaCompleta){

                SDL_Texture * outTexture = SDL_CreateTexture(mGRenderer,SDL_PIXELFORMAT_ARGB4444,SDL_TEXTUREACCESS_TARGET,mRectAreaJuego.w,mSizeCuadro);
                if(outTexture == nullptr){
                    std::cerr << "No se logró crear la textura Error: " << SDL_GetError() << std::endl;
                }
                if(SDL_SetRenderTarget(mGRenderer,outTexture) < 0){
                    std::cerr << "No se logró establecer el Target outTexture: " << SDL_GetError() << std::endl;
                }

                for(int j = 0; j < mColumnas ;j++){
                    mSpriteSheetBloques->setCurrentCuadro(mArrayEstadoJuegoTetrominosCaidos[i*mColumnas + j] - 1);
                    mSpriteSheetBloques->draw(mGRenderer,j*mSizeCuadro,0);
                    mArrayEstadoJuegoTetrominosCaidos[i*mColumnas + j] = 0;
                }

                mControlAnimaciones->add(GRUPO_ANIMACION_ELIMINAR_LINEAS, new AnimacionDesvanecerOut(outTexture, mRectAreaJuego.x, i * mSizeCuadro + mRectAreaJuego.y, 2,30));
                SDL_SetRenderTarget(mGRenderer, nullptr);
                nLineasEliminadas++;
            }
        }
        return nLineasEliminadas;
    }


    void estadoRunningUpdate(){

        mControlAnimaciones->update();

        if(mDequeTetraminosCayendo.size() > 0) {

            mActualDelayBajarTetraminosFlotantes++;

            if(mActualDelayBajarTetraminosFlotantes >= mMaxDelayBajarTetraminosFlotantes){
                mActualDelayBajarTetraminosFlotantes = 0;

                bool almenosUnTetraminoTocoPiso = false;

                auto itBloqueAnimacionCayendo = mDequeTetraminosCayendo.begin();
                while (itBloqueAnimacionCayendo != mDequeTetraminosCayendo.end()) {

                    (*itBloqueAnimacionCayendo)->moveIp(0, mSizeCuadro);
                    if (estaTetraminoEnPosicionInvalida((*itBloqueAnimacionCayendo))) {
                        (*itBloqueAnimacionCayendo)->moveIp(0, -mSizeCuadro);
                        guardarTetrominoEnFondo((*itBloqueAnimacionCayendo));
                        itBloqueAnimacionCayendo = mDequeTetraminosCayendo.erase(itBloqueAnimacionCayendo);
                        almenosUnTetraminoTocoPiso = true;
                    } else {
                        ++itBloqueAnimacionCayendo;
                    }
                }

                if(almenosUnTetraminoTocoPiso) {
                    int nLineasCompletas = limpiarLineasCompletas();
                    if(nLineasCompletas){
                        mParent->tetrisLineasCompletadas(mId,nLineasCompletas);
                    }
                }

                if(mDequeTetraminosCayendo.size() == 0 &&
                   mControlAnimaciones->numeroAnimacionesActivas(GRUPO_ANIMACION_ELIMINAR_LINEAS) == 0) {
                    crearTetrominos();
                }
            }

        }

        if(mTetrominoActual != nullptr){

            const Uint8 *teclas= SDL_GetKeyboardState(NULL);//se obtiene el estado_actual actual del teclado
            mControlPlayerTeclas->update(teclas);
            //mTetrominoActual->update(nullptr);

            mDelayActualBajarTetrominoActual++;

            if(mControlPlayerTeclas->estaTeclaAceptada(ControlTeclasPlayer::TECLA_MOVER_LEFT)){
                if (moverIpTetromino(mTetrominoActual, -mSizeCuadro, 0)) {
//                    mParent->playSfx(mSfxPieceMove);
                    mDelayActualBajarTetrominoActual = 0;
                } else {
//                    mParent->playSfx(mSfxPieceTouch);
                }
            }else if(mControlPlayerTeclas->estaTeclaAceptada(ControlTeclasPlayer::TECLA_MOVER_RIGHT)){
                if (moverIpTetromino(mTetrominoActual, mSizeCuadro, 0)) {
//                    mParent->playSfx(mSfxPieceMove);
                    mDelayActualBajarTetrominoActual = 0;
                } else {
//                    mParent->playSfx(mSfxPieceTouch);
                }
            }else if(mControlPlayerTeclas->estaTeclaAceptada(ControlTeclasPlayer::TECLA_HARD_DROP)) {
                int nCeldasDropeadas = hardDropTetramino(mTetrominoActual);
                if(nCeldasDropeadas > 0){
                    mParent->tetrisHardDrop(mId,nCeldasDropeadas);
                }

                mDelayActualBajarTetrominoActual = mMaxDelayBajarTetrominoActual + 1;
//                mParent->playSfx(mSfxHoldPiece);
            }else if(mControlPlayerTeclas->estaTeclaPresionada(ControlTeclasPlayer::TECLA_SOFT_DROP) &&
                    !mEstaUtilizandoSoftDrop) {
                    mDelayActualBajarTetrominoActual = mMaxDelayBajarTetrominoActual + 1;
                    mMaxDelayBajarTetrominoActual /= 4;
//                    mParent->playSfx(mSfxSoftDrop);
                    mCeldasAvanzadasSoftDrop = 0;
                    mEstaUtilizandoSoftDrop = true;

            }else if(!mControlPlayerTeclas->estaTeclaPresionada(ControlTeclasPlayer::TECLA_SOFT_DROP) &&
                    mEstaUtilizandoSoftDrop) {
                mMaxDelayBajarTetrominoActual *= 4;
//                mParent->playSfx(mSfxHardDrop);
                if (mCeldasAvanzadasSoftDrop) {
                    mParent->tetrisSoftDrop(mId, mCeldasAvanzadasSoftDrop);
                    mCeldasAvanzadasSoftDrop = 0;
                }
                mEstaUtilizandoSoftDrop = false;
            }else if(mControlPlayerTeclas->estaTeclaAceptada(ControlTeclasPlayer::TECLA_GIRAR_LEFT)) {
                if (girarFormaActualDesc(1)) {
                    mDelayActualBajarTetrominoActual = 0;
                    mTetrominoGhost->rotate(1);
                }
            }else if(mControlPlayerTeclas->estaTeclaAceptada(ControlTeclasPlayer::TECLA_GIRAR_RIGHT)) {
                if (girarFormaActualDesc(-1)) {
                    mTetrominoGhost->rotate(-1);
                    mDelayActualBajarTetrominoActual = 0;
                }
            }else if(mControlPlayerTeclas->estaTeclaAceptada(ControlTeclasPlayer::TECLA_PAUSA)) {
                mParent->tetrisPaused(mId);
                mEstadoJuego = EstadoJuego ::PAUSADO;
                mOpcionSeleccionadaMenuPausa = 0;
                mpBitFntRendOpsMenuPausa[mOpcionSeleccionadaMenuPausa]->setBitmapFont(mpBitmapFont[EstadoOpcionMenu::RESALTADO]);
            }

            mTetrominoGhost->move(mTetrominoActual->getX(),mTetrominoActual->getY());
            hardDropTetramino(mTetrominoGhost);

            if(mDelayActualBajarTetrominoActual >= mMaxDelayBajarTetrominoActual){
                mDelayActualBajarTetrominoActual = 0;

                if(!moverIpTetromino(mTetrominoActual, 0, mSizeCuadro)){
                    if(mEstaUtilizandoSoftDrop){
                        if(mCeldasAvanzadasSoftDrop){
                            mParent->tetrisSoftDrop(mId,mCeldasAvanzadasSoftDrop);
                            mCeldasAvanzadasSoftDrop = 0;
                        }
                    }

//                    mParent->playSfx(mSfxPieceLockDown);
                    guardarTetrominoEnFondo(mTetrominoActual);

                    delete mTetrominoActual;
                    mTetrominoActual = nullptr;

                    int lineasCompletas = limpiarLineasCompletas();
                    if( lineasCompletas== 0){
                        crearTetrominos();
                    }else{
                        mParent->tetrisLineasCompletadas(mId,lineasCompletas);
                    }
                }else{
                    if(mEstaUtilizandoSoftDrop){
                        mCeldasAvanzadasSoftDrop++;
                    }

//                    mParent->playSfx(mSfxPieceFall);
                }

            }

        }
    }

    void estadoGameOverUpdate(){
    }

    void estadoPausadoUpdate(){
    }


    void update() {

        switch(mEstadoJuego){

            case STOPPED:break;
            case RUNNING:
                estadoRunningUpdate();
                break;
            case PAUSADO:
                estadoPausadoUpdate();
                break;
            case GAME_OVER:
                estadoGameOverUpdate();
                break;
        }
        //mSprites->update(nullptr);
    }

    bool existeIndice(int i,int j){
        return i>= 0&& i < mFilas && j>=0 && j < mColumnas;
    }

    int getValorJuegoArray(int i, int j){
        return mArrayEstadoJuegoTetrominosCaidos[i*mColumnas + j];
    }

    void extraerTetrominoRecursionAux(int i_fondo, int j_fondo, Tetromino *forma){

        if(mArrayEstadoJuegoTetrominosCaidos[i_fondo*mColumnas + j_fondo] == 0) return;

        bool existeExtArriba    = existeIndice(i_fondo-1, j_fondo)   && getValorJuegoArray(i_fondo - 1, j_fondo) != 0;
        bool existeExtAbajo     = existeIndice(i_fondo+1, j_fondo)   && getValorJuegoArray(i_fondo + 1, j_fondo) != 0;
        bool existeExtIzquierda = existeIndice(i_fondo,   j_fondo-1) && getValorJuegoArray(i_fondo, j_fondo - 1) != 0;
        bool existeExtDerecha   = existeIndice(i_fondo,   j_fondo+1) && getValorJuegoArray(i_fondo, j_fondo + 1) != 0;

        forma->setValorIndice(i_fondo,j_fondo,mArrayEstadoJuegoTetrominosCaidos[i_fondo*mColumnas + j_fondo]);

        mArrayEstadoJuegoTetrominosCaidos[i_fondo*mColumnas + j_fondo] = 0;

        if(existeExtArriba){
            extraerTetrominoRecursionAux(i_fondo - 1, j_fondo, forma);
        }
        if(existeExtAbajo) {
            extraerTetrominoRecursionAux(i_fondo + 1, j_fondo, forma);
        }
        if(existeExtDerecha) {
            extraerTetrominoRecursionAux(i_fondo, j_fondo + 1, forma);
        }
        if(existeExtIzquierda) {
            extraerTetrominoRecursionAux(i_fondo, j_fondo - 1, forma);
        }
    }

    Tetromino * extraerTetrominoDelFondo(int iInicio, int jInicio){
        Tetromino * nuevaForma = new Tetromino(mFilas,mColumnas,mRectAreaJuego.x,mRectAreaJuego.y,mSizeCuadro,mSpriteSheetBloques);
        extraerTetrominoRecursionAux(iInicio, jInicio, nuevaForma);
        return nuevaForma;
    }

    void guardarTetrominoEnFondo(Tetromino *pPieza){

        int indiceArray;

        for(int i = 0; i < pPieza->getNFilas(); i++ ){
            for(int j = 0; j < pPieza->getNColumnas();j++){
                if(pPieza->valorIndiceAC(i, j)!= 0){
                    indiceArray = ((i*mSizeCuadro + pPieza->getY()) - mRectAreaJuego.y)/mSizeCuadro*mColumnas + ((j*mSizeCuadro + pPieza->getX() - mRectAreaJuego.x)/mSizeCuadro);
                    mArrayEstadoJuegoTetrominosCaidos[indiceArray] = pPieza->valorIndiceAC(i, j);
                }
            }
        }

    }
    bool estaTetrominoFlotando(Tetromino *forma){
        for(int i = 0; i < forma->getNFilas() ; i ++){
            for(int j = 0; j < forma->getNColumnas();j++){
                if(forma->valorIndiceAC(i,j) != 0){
                    int indice_i = ((i + 1)* mSizeCuadro+ forma->getY() - mRectAreaJuego.y)/mSizeCuadro;
                    int indice_j = (j* mSizeCuadro+ forma->getX() - mRectAreaJuego.x)/mSizeCuadro;
                    if((existeIndice(indice_i,indice_j) && getValorJuegoArray(indice_i, indice_j) != 0) ||
                        !existeIndice(indice_i,indice_j)){
                        return false;
                    }
                }
            }
        }
        return true;
    }
    
    void establecerPosicionesVisitadasRecursion(Tetromino * pPosicionesVisitadas, Uint8 nuevoValor){
        int indiceArray;

        for(int i = 0; i < pPosicionesVisitadas->getNFilas(); i++ ){
            for(int j = 0; j < pPosicionesVisitadas->getNColumnas();j++){
                if(pPosicionesVisitadas->valorIndiceAC(i, j)!= 0){

                    indiceArray = ((i*mSizeCuadro + pPosicionesVisitadas->getY()) - mRectAreaJuego.y)/mSizeCuadro*mColumnas + ((j*mSizeCuadro + pPosicionesVisitadas->getX() - mRectAreaJuego.x)/mSizeCuadro);

                    mArrayEstadoJuegoRecursion[indiceArray] = nuevoValor;
                }
            }
        }
        
    }
    void bajarLineasFlotantes() {

        std::fill_n(mArrayEstadoJuegoRecursion, mFilas*mColumnas, POS_NO_VISITADA_RECURSION);

        bool encontradaPiezaFlotante;

        do {
            encontradaPiezaFlotante = false;
            for (int i = mFilas - 2; i >=0; i--) {
                for (int j = 0; j < mColumnas; j++) {

                    if (mArrayEstadoJuegoTetrominosCaidos[i * mColumnas + j] != 0 &&
                        mArrayEstadoJuegoRecursion[i*mColumnas + j] == POS_NO_VISITADA_RECURSION) {

                        Tetromino *pPiezaExtraida = extraerTetrominoDelFondo(i, j);

                        if (estaTetrominoFlotando(pPiezaExtraida)) {
                            mDequeTetraminosCayendo.push_back(pPiezaExtraida);
                            encontradaPiezaFlotante = true;
                            establecerPosicionesVisitadasRecursion(pPiezaExtraida, POS_VISITADA_RECURSION);
                        } else {
                            guardarTetrominoEnFondo(pPiezaExtraida);
                            establecerPosicionesVisitadasRecursion(pPiezaExtraida, POS_VISITADA_RECURSION);
                            delete pPiezaExtraida;
                        }
                    }

                }
            }
        }while(encontradaPiezaFlotante);

        if(mDequeTetraminosCayendo.size() > 0){
            mActualDelayBajarTetraminosFlotantes = 0;
            mMaxDelayBajarTetraminosFlotantes = 10;
        }else{
            crearTetrominos();
        }
    }

    void stopped(const int GrupoId) override {
        switch(GrupoId){
            case GRUPO_ANIMACION_ELIMINAR_LINEAS:
                bajarLineasFlotantes();
                //crearTetrominos();
                break;
            default:break;
        }
    }

    ~TetrisJuego(){
        //delete mSprites;
        //delete mTextureBackground;

		delete mArrayEstadoJuegoTetrominosCaidos;
		delete mArrayEstadoJuegoRecursion;
        delete mControlPlayerTeclas;
		delete mControlAnimaciones;
        delete mSpriteSheetBloques;
        delete mTetrominoActual;
        delete mTetrominoSiguiente;
		delete mTetrominoGhost;

        for(int i = 0; i < 2 ;i++){
            delete mpBitmapFont[i];
        }

        for(int i = 0; i < N_OPCIONES_PAUSA;i++){
            delete mpBitFntRendOpsMenuPausa[i];
        }
        delete plTextureFlechaOpcionSeleccionada;
        delete plTextureGameOver;

//        Mix_FreeChunk(mSfxPieceRotate);
//        Mix_FreeChunk(mSfxPieceRotateFail);
//        Mix_FreeChunk(mSfxPieceTouch);
//        Mix_FreeChunk(mSfxPieceLockDown);
//        Mix_FreeChunk(mSfxPieceFall);
//        Mix_FreeChunk(mSfxPieceMove);
//        Mix_FreeChunk(mSfxSoftDrop);
//        Mix_FreeChunk(mSfxHardDrop);
//        Mix_FreeChunk(mSfxHoldPiece);

		for (auto & tetrominoCayendo : mDequeTetraminosCayendo) {
			delete tetrominoCayendo;
		}
    }


    int getTickDelayBajarTetromino() {
        return mMaxDelayBajarTetrominoActual;
    }

    void setTickDelayBajarTetromino(int nuevoTickEspera) {
        mDelayActualBajarTetrominoActual = 0;
        mMaxDelayBajarTetrominoActual = nuevoTickEspera;
    }

private:

	enum EstadoJuego {
		STOPPED,
		RUNNING,
		PAUSADO,
		GAME_OVER
	};
	enum EstadoOpcionMenu {
		NORMAL,
		RESALTADO
	};
	enum OpcionesPausa {
		CONTINUE,
		RETRY,
		END,
		N_OPCIONES_PAUSA
	};
	
	InterfazJuegoTetris * mParent = nullptr;
	SDL_Renderer *mGRenderer = nullptr;
	
	int mId;
	EstadoJuego mEstadoJuego = EstadoJuego::STOPPED;

	SDL_Rect  mRectAreaJuego    {0,0,0,0};

    Uint8 * mArrayEstadoJuegoTetrominosCaidos = nullptr;
    Uint8 * mArrayEstadoJuegoRecursion = nullptr;

    int mFilas      = 0;
    int mColumnas   = 0;
    int mSizeCuadro = 0;

    Tetromino * mTetrominoActual = nullptr;
	Tetromino * mTetrominoSiguiente = nullptr;
	Tetromino * mTetrominoGhost = nullptr;

	int mDelayActualBajarTetrominoActual = 0;
    int mMaxDelayBajarTetrominoActual = 0;

    int mActualDelayBajarTetraminosFlotantes = 0;
    int mMaxDelayBajarTetraminosFlotantes = 0;

    //bool mMantieneTeclaLeftPresionada = false;
    //bool mMantieneTeclaRightPresionada = false;
    //bool mMantieneTeclaGiroCounterClockPresionada = false;
    //bool mMantieneTeclaHardDropPresionada = false;
    //bool mMantieneTeclaSoftDropPresionada = false;
    //bool mMantieneTeclaPausaPresionada = false;

    SpriteSheet * mSpriteSheetBloques = nullptr;
    //LTexture *mTextureBackground;
    ControlaAnimacion * mControlAnimaciones = nullptr;

	std::deque<Tetromino *>  mDequeTetraminosCayendo;

    BitmapFont * mpBitmapFont[2]  {nullptr};
    BitmapFontRenderer * mpBitFntRendOpsMenuPausa[N_OPCIONES_PAUSA] {nullptr};

    LTexture * plTextureFlechaOpcionSeleccionada = nullptr;
	LTexture * plTextureGameOver = nullptr;

    int mOpcionSeleccionadaMenuPausa = 0;

//
//    Mix_Chunk * mSfxPieceRotate = nullptr;
//    Mix_Chunk * mSfxPieceRotateFail = nullptr;
//    Mix_Chunk * mSfxPieceMove = nullptr;
//    Mix_Chunk * mSfxPieceFall = nullptr;
//    Mix_Chunk * mSfxPieceLockDown = nullptr;
//    Mix_Chunk * mSfxPieceTouch = nullptr;
//
//    Mix_Chunk * mSfxSoftDrop = nullptr;
//    Mix_Chunk * mSfxHardDrop = nullptr;
//    Mix_Chunk * mSfxHoldPiece = nullptr;

    int mCeldasAvanzadasSoftDrop = 0;
    bool mEstaUtilizandoSoftDrop = false;

	RandomGenerator mRandomGenerator;
    ControlTeclasPlayer * mControlPlayerTeclas = nullptr;
};
#endif //TETRIS_TETRISJUEGO_HPP
//
//    int obtenerIndiceBloqueSpriteSheet(int i, int j, Tetromino * pPieza) {
//
//        bool existeExtArriba = pPieza->existeIndiceAC(i - 1, j) &&
//                                     pPieza->valorIndiceAC(i-1,j) != 0;
//        bool existeExtAbajo = pPieza->existeIndiceAC(i + 1, j) &&
//                                     pPieza->valorIndiceAC(i+1,j) != 0;
//        bool existeExtIzquierda = pPieza->existeIndiceAC(i, j - 1) &&
//                                     pPieza->valorIndiceAC(i,j-1) != 0;
//        bool existeExtDerecha = pPieza->existeIndiceAC(i, j + 1) &&
//                                     pPieza->valorIndiceAC(i,j+1) != 0;
//        int indice = obtenerIndiceBloqueSpriteSheet(existeExtArriba, existeExtAbajo, existeExtIzquierda, existeExtDerecha);
//        switch (indice){
//            case 3*8 + 2 + 1:
//                if(pPieza->existeIndiceAC(i,j-2) &&pPieza->valorIndiceAC(i,j-2) == 0)
//                    return 2*8 + 2 + 1;
//            case 2*8 + 1 + 1:
//                if(pPieza->existeIndiceAC(i,j-2) &&pPieza->valorIndiceAC(i,j-2) == 0)
//                    return 3*8 + 1 + 1;
//            default:
//                return indice;
//        }
//
//    }
//
//    int obtenerIndiceBloqueSpriteSheet(bool existeExtArriba, bool existeExtAbajo, bool existeExtIzquierda,
//                                       bool existeExtDerecha){
//        if(existeExtArriba && existeExtDerecha && existeExtIzquierda){
//            return 3*8 + 5 + 1;
//        }else if(existeExtAbajo && existeExtDerecha && existeExtIzquierda){
//            return  3*8 + 4 + 1;
//        }else if(existeExtAbajo && existeExtArriba){
//            return 1*8 + 6 + 1;
//        }else if(existeExtDerecha && existeExtIzquierda){
//            return 0*8 + 7 + 1;
//        }else if(existeExtDerecha && existeExtAbajo){
//            return 2*8 + 4 + 1;
//        }else if(existeExtDerecha && existeExtArriba){
//            return 4*8 + 2 + 1;
//        }else if(existeExtIzquierda && existeExtArriba){
//            return 2*8 + 7 + 1;
//        }else if(existeExtIzquierda && existeExtAbajo){
//            return 4*8 + 1 + 1;
//        }else if(existeExtIzquierda){
//            return 3*8 + 2 + 1;
//        }else if(existeExtDerecha){
//            return 3*8 + 0 + 1;
//        }else if(existeExtArriba){
//            return 2*8 + 1 + 1;
//        }else if(existeExtAbajo){
//            return 2*8 + 3 + 1;
//        }else{
//            return 1*8 + 1 + 1;
//        }
//    }
//
//    void obtenerExtensionesIndiceSpriteSheet(int indice,
//                                       bool & deberiaExistirArriba,    bool & deberiaExistirAbajo,
//                                       bool & deberiaExistirIzquierda, bool & deberiaExistirDerecha){
//        deberiaExistirArriba = false;
//        deberiaExistirDerecha = false;
//        deberiaExistirIzquierda = false;
//        deberiaExistirAbajo = false;
//
//        switch(indice){
//            case 3*8 + 5 + 1:
//                deberiaExistirArriba = true;
//                deberiaExistirDerecha = true;
//                deberiaExistirIzquierda = true;
//                break;
//            case 3*8 + 4 + 1:
//                deberiaExistirAbajo = true;
//                deberiaExistirDerecha = true;
//                deberiaExistirIzquierda = true;
//                break;
//            case 1*8 + 6 + 1:
//                deberiaExistirAbajo = true;
//                deberiaExistirArriba = true;
//                break;
//            case 0*8 + 7 + 1:
//                deberiaExistirDerecha = true;
//                deberiaExistirIzquierda = true;
//                break;
//            case 2*8 + 4 + 1:
//                deberiaExistirDerecha = true;
//                deberiaExistirAbajo = true;
//                break;
//            case 4*8 + 2 + 1:
//                deberiaExistirDerecha = true;
//                deberiaExistirArriba = true;
//                break;
//            case 2*8 + 7 + 1:
//                deberiaExistirArriba = true;
//                deberiaExistirIzquierda = true;
//                break;
//            case 4*8 + 1 + 1:
//                deberiaExistirAbajo = true;
//                deberiaExistirIzquierda = true;
//                break;
//            case 3*8 + 2 + 1:
//            case 2*8 + 2 + 1:
//                deberiaExistirIzquierda = true;
//                break;
//            case 3*8 + 0 + 1:
//                deberiaExistirDerecha = true;
//                break;
//            case 2*8 + 1 + 1:
//            case 3*8 + 1 + 1:
//                deberiaExistirArriba = true;
//                break;
//            case 2*8 + 3 + 1:
//                deberiaExistirAbajo = true;
//                break;
//            default:
//                break;
//
//        }
//    }
