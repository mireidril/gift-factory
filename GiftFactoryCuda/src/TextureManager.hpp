#ifndef __TEXTURE_MANAGER_HPP__
#define __TEXTURE_MANAGER_HPP__

#include <iostream>
#include <vector>
#include <sstream>
#ifdef _WIN32
#include "SDL/SDL_image.h"
#else
#include <SDL/SDL_image.h>
#endif

class TextureManager
{
	public :
		struct Texture
		{
			std::string texFileName;
			std::string shaderUniformName;
			SDL_Surface * texPicture;
		};

		static Texture* getTextIfExist(std::string name);
		static Texture* addAndLoadTexture(std::string name, std::string shaderUniformName);

	private :
		static std::vector<Texture *> m_textures;
};

#endif
