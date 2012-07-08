/*#ifndef TEXENGINE_TEXTURE_HPP_INCLUDED
#define TEXENGINE_TEΧΤURE_HPP_INCLUDED*/

#ifndef TEXENGINE_TEXTURE_HPP_INCLUDED
#define TEXENGINE_TEXTURE_HPP_INCLUDED

#include "../math/vector.hpp"
#include "../port/gfxapi.h"
#include "../util/image.hpp"
#include "assetEnum.h"

using TEXEngine::Math::Vector2f;
using TEXEngine::Util::Image;
using TEXEngine::Port::GFXAPIuint;

namespace TEXEngine {

	namespace Core {
		class AssetManager; /* Forward declaration */
	}
	namespace Port {
		class PCRenderer; /* Forward declaration */
	}

	using Core::AssetManager;
	using Port::PCRenderer;
	
	namespace Asset {

class Texture
{
	friend class PCRenderer;
	friend class AssetManager;
	protected:
		Texture();
		Texture(const Texture &tex);
		Texture &operator =(const Texture &tex);
		~Texture();

		void id(GFXAPIuint id);

	public:
		TEXENG_TEXTURE_TYPE type() const;
		GFXAPIuint id() const;
		void type(TEXENG_TEXTURE_TYPE type);

		const Vector2f &offset();
		void offset(const Vector2f &offset);

		Image img;

	private:
		Vector2f m_offset;
		TEXENG_TEXTURE_TYPE m_type;
		GFXAPIuint m_id;
};

	} /* namespace Asset */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_TEXTURE_HPP_INCLUDED */