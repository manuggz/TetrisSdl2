//
// Created by manuggz on 18/11/16.
//

#include "LTexture.hpp"

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}

bool LTexture::cargarDesdeArchivo(std::string path, SDL_Renderer *gRenderer, bool tiene_color_clave) {
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    mSurface = IMG_Load( path.c_str() );
    if( mSurface == NULL ){
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"Unable to load image %s! SDL_image Error: %s",
                    path.c_str(),
                    IMG_GetError() );
        return false;
    }else{

        if(tiene_color_clave){
            //Color key image
            SDL_SetColorKey( mSurface, SDL_TRUE,get_pixel(mSurface,0,0 ) );
        }

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, mSurface );
        if( newTexture == NULL )
        {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"Unable to create texture from %s! SDL Error: %s",
                        path.c_str(),
                        SDL_GetError() );
            return false;
        }
        else
        {
            SDL_Log("Cargada Textura : %s.",path.c_str());
            //Get image dimensions
            mWidth = mSurface->w;
            mHeight = mSurface->h;
        }

    }

    //Return success
    mTexture = newTexture;
    mPath = path;
    return mTexture != NULL;
}
bool LTexture::loadFromRenderedText(SDL_Renderer * gRenderer, TTF_Font * gFont, std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "Unable to create texture from text: %s! SDL Error: %s",
                        textureText.c_str(),
                        SDL_GetError() );
            //Get rid of old surface
            SDL_FreeSurface( textSurface );
            return false;
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

	}
	else
	{
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,"Unable to draw text surface! SDL_ttf Error: %s.", TTF_GetError() );
        return false;
	}


	//Return success
    mPath = "";
	return mTexture != nullptr;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != nullptr )
    {
        SDL_DestroyTexture( mTexture );
        //Get rid of old loaded surface
        SDL_FreeSurface( mSurface );
        mTexture = nullptr;
        mSurface = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
    //Modulate texture rgb
    SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    //Set blending function
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::draw(SDL_Renderer *gRenderer, int x, int y,
                    SDL_Rect *clip, double angle, SDL_Point *center,
                    SDL_RendererFlip flip) {
    if(mTexture == nullptr) return;
    //Set rendering space and draw to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}



int LTexture::getWidth()const {
    return mWidth;
}

int LTexture::getHeight()const {
    return mHeight;
}

std::string LTexture::getPath()const {
    return mPath;
}

