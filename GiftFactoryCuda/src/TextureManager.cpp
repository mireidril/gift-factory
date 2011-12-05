#include "TextureManager.hpp"

std::vector<TextureManager::Texture *> TextureManager::m_textures;

TextureManager::Texture* TextureManager::getTextIfExist(std::string name){
	for(std::vector<Texture *>::iterator it = m_textures.begin(); it!=m_textures.end(); ++it) {
		if( (*it)->texFileName == name)
			return (*it);
	}
	return 0;
}

TextureManager::Texture* TextureManager::addAndLoadTexture(std::string name, std::string shaderUniformName){
	// Init the texture data
	Texture *tex = new Texture();
	tex->texFileName = name;
	tex->shaderUniformName = shaderUniformName;
	// Load the texture image
	std::string texFile = "./textures/" + name;
	std::cout << "loading : " << texFile << std::endl;
	SDL_Surface *surf = IMG_Load(texFile.c_str());
	tex->texPicture = surf;
	// Add the texture to the list
	m_textures.push_back(tex);

	return m_textures[m_textures.size()-1];
}
