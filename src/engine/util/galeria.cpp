//#include "galeria.hpp"
//#include "LTexture.hpp"
//
//
//Galeria::Galeria(){
//    #ifdef DEBUG
//    cout << "Constructor de galeria:"<<this<<endl;
//    #endif
//
//    sonidoCargado=false;
//
//}
//
//bool Galeria::cargarTexturas(SDL_Renderer *gRenderer, string rutaTexturas) {
//    string ruta;
//    bool keyColor;
//
//    ifstream filePar(rutaTexturas);
//    if(!filePar){
//        cerr << "No se pudo abrir archivo con texturas: " << rutaTexturas << endl;
//        return false;
//    }
//
//    for(int j=0;j<_IMAGENES;j++){
//        filePar >>ruta;
//        filePar >>keyColor;
//        baulimgs[j] = new LTexture();
//        baulimgs[j]->cargarDesdeArchivo(ruta, gRenderer, keyColor);
//        cout << "+Texture(Path = " << baulimgs[j]->getPath()
//                << " Height: "<<baulimgs[j]->getHeight()
//                << ", stWidth:" << baulimgs[j]->getWidth() <<endl;
//    }
//    filePar.close();
//
//    return true;
//}
//
//bool Galeria::cargarSonidos(string rutaSonidos) {
//
//    std::cout << "+ cargando sonidos en:"<<rutaSonidos<<std::endl;
//    std::string ruta_sonido;
//
//    for(int i=0;i<_SONIDOS;i++){
//        ruta_sonido = rutaSonidos + "musica_" + std::to_string(i + 1) +  ".mid";
//        snd_musicas[i]= Mix_LoadMUS(ruta_sonido.c_str());
//        if(snd_musicas[i] != nullptr){
//            std::cout << "+ [INFO]  cargado: " << ruta_sonido << std::endl;
//        }else{
//            std::cerr << "- [ERROR] cargando:" << ruta_sonido << Mix_GetError()<<std::endl;
//        }
//    }
//
//    for(int i=0;i<_EFECTOS;i++){
//        ruta_sonido = rutaSonidos + "ping_" + std::to_string(i + 1) +  ".wav";
//
//        sfx_efectos[i]=Mix_LoadWAV(ruta_sonido.c_str());
//        if(sfx_efectos[i] != nullptr){
//            std::cout << "+ cargado: "<<ruta_sonido<<std::endl;
//            Mix_VolumeChunk(sfx_efectos[i], 100);
//        }else{
//            std::cerr<<" - [ERROR] cargando:"<<ruta_sonido<<Mix_GetError()<<std::endl;
//        }
//    }
//
//    Mix_VolumeMusic(128);
//
//    return true;
//}
//
//Galeria::~Galeria(){
//    #ifdef DEBUG
//    cout << "Destructor de galeria:"<<this<<endl;
//    #endif
//
//    for(int i=0;i<_IMAGENES;i++){
//        baulimgs[i]->free();
//        delete baulimgs[i];
//    }
//
//    if(sonidoCargado){
//         for(int i=0;i<_SONIDOS;i++)Mix_FreeMusic(snd_musicas[i]);
//         for(int i=0;i<_EFECTOS;i++)Mix_FreeChunk(sfx_efectos[i]);
//    }
//}
//
