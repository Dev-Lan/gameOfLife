#pragma once
#include <G3D/G3DAll.h>

class AttemptTexture {
public:
	
	static inline shared_ptr< Texture > attemptTextureFromFile(const String& filename, Texture::Encoding encoding=Texture::Encoding(), Texture::Dimension dimension=Texture::DIM_2D, bool generateMipMaps=true, const Texture::Preprocess &preprocess=Texture::Preprocess::defaults(), bool preferSRGBForAuto=false) {
		shared_ptr< Texture > result;
		try {
			result = Texture::fromFile(filename, encoding, dimension, generateMipMaps, preprocess, preferSRGBForAuto);
		} catch (G3D::Image::Error& e) {
			std::cerr << "ERROR : Could not create texture from file : " << filename << std::endl;
			result = Texture::createEmpty("",1,1);
		}
		return result;
}
};

	