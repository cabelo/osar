/* -*-c++-*- 
 * 
 * osgART - ARToolKit for OpenSceneGraph
 * Copyright (C) 2005-2008 Human Interface Technology Laboratory New Zealand
 * 
 * This file is part of osgART 2.0
 *
 * osgART 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * osgART 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with osgART 2.0.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "OpenThreads/ScopedLock"
#include "osgART/Video"

#include "osgART/VideoGeode"

#include "osgART/Utils"

#include <osg/Node>
#include <osg/Texture>
#include <osg/Texture2D>
#include <osg/TextureRectangle>
#include <osg/BlendFunc>
#include <osg/Notify>
#include <osg/Geometry>
#include <osg/ImageStream>

#include "osgART/ImageStreamCallback"


namespace osgART {

	namespace Internal {

		class Texture2DCallback : public osg::Texture2D::SubloadCallback
		{
		public:
			
			Texture2DCallback(osg::Texture2D* texture);

			void load(const osg::Texture2D& texture, osg::State&) const;
			void subload(const osg::Texture2D& texture, osg::State&) const;

			inline float getTexCoordX() const { return (_texCoordX);};
			inline float getTexCoordY() const { return (_texCoordY);};

		protected:
			
			float _texCoordX;
			float _texCoordY;

		};


		Texture2DCallback::Texture2DCallback(osg::Texture2D* texture) : osg::Texture2D::SubloadCallback(),
			_texCoordX(texture->getImage()->s() / (float)nextPowerOfTwo((unsigned int)texture->getImage()->s())),
			_texCoordY(texture->getImage()->t() / (float)nextPowerOfTwo((unsigned int)texture->getImage()->t()))
		{
			texture->setTextureSize(nextPowerOfTwo((unsigned int)texture->getImage()->s()),
				nextPowerOfTwo((unsigned int)texture->getImage()->t()));

			texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
			texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
			texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::CLAMP);
			texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::CLAMP);
			
		}

		/*virtual*/ 
		void 
		Texture2DCallback::load(const osg::Texture2D& texture, osg::State& state) const 
		{
			
			const osg::Image* _image = texture.getImage();
#if 1
			glTexImage2D(GL_TEXTURE_2D, 0, 
				// hse25: internal texture format gets overwritten by the image format 
				// we need just the components - ???
				osg::Image::computeNumComponents(_image->getInternalTextureFormat()), 
				(float)nextPowerOfTwo((unsigned int)_image->s()), 
				(float)nextPowerOfTwo((unsigned int)_image->t()), 
				0, _image->getPixelFormat(), 
				_image->getDataType(), 0);
#else
			texture.applyTexImage2D_load(state, GL_TEXTURE_2D, _image, 
				_image->s(),
				_image->t(),
				//(float)nextPowerOfTwo((unsigned int)_image->s()), 
				//(float)nextPowerOfTwo((unsigned int)_image->t()),
				1);

#endif

		}
		
		/*virtual */ 
		void
		Texture2DCallback::subload(const osg::Texture2D& texture, osg::State& state) const 
		{
			const osg::Image* _image = texture.getImage();
			texture.applyTexImage2D_subload(state, GL_TEXTURE_2D, _image, _image->s(), _image->t(), _image->getInternalTextureFormat(), 1);
		}
	}

	VideoGeode::VideoGeode(TextureMode texturemode /*= USE_TEXTURE_2D*/, osg::Image* image/* = 0L*/) : 
		_textureMode(texturemode)

	{
		this->addImage(image);
	}

	/*virtual */
	void VideoGeode::addImage(osg::Image* image)
	{
		if (!image) return;
		if (!image->valid()) return;

		_images.push_back(image);

		// some extra functionality for ImageStreams
		osg::ImageStream* imagestream = dynamic_cast<osg::ImageStream*>(image);

		if (imagestream)
		{
			ImageStreamCallback* callback = new ImageStreamCallback(imagestream);

			this->getEventCallback() ? this->getEventCallback()->addNestedCallback(callback) :
				this->setEventCallback(callback);
		}

		this->computeTextures();
	}



	VideoGeode::VideoGeode(const VideoGeode&, const osg::CopyOp& /*copyop = CopyOp::SHALLOW_COPY*/)
	{
	}


	void VideoGeode::setAlpha(float alpha) 
	{
		if (alpha < 1.0f) //if no transparency, non activate blending op (if already activate, override)
		{
			osg::BlendFunc* blendFunc = new osg::BlendFunc();
			blendFunc->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
		
			osg::StateSet* stateset = this->getStateSet();
		
			stateset->setAttribute(blendFunc);
			stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
			stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
		}

		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,alpha));


		for (osg::Geode::DrawableList::iterator i = _drawables.begin();i != _drawables.end();i++)
		{

			osg::Geometry* geom = dynamic_cast<osg::Geometry*>((*i).get());

			if (geom)
			{
				geom->setColorArray(colors);
				geom->setColorBinding(osg::Geometry::BIND_OVERALL);
			}			
		}
	}



	void VideoGeode::computeTextures()
	{

		// loop through Images
		for (ImageArray::iterator image = _images.begin(); image != _images.end(); image++)
		{

			osg::Texture* texture;
			
			float maxU = 1.0f, maxV = 1.0f;

			switch (_textureMode) 
			{
				case USE_TEXTURE_RECTANGLE:
					
					osg::notify() << "osgART::VideoGeode() using TextureRectangle" << std::endl;

					texture = new osg::TextureRectangle((*image).get());
					
					break;

				default:
				case USE_TEXTURE_2D:			

					osg::notify() << "osgART::VideoGeode() using Texture2D" << std::endl;

					texture = new osg::Texture2D((*image).get());

					Internal::Texture2DCallback *_cb = 
						new Internal::Texture2DCallback(dynamic_cast<osg::Texture2D*>(texture));

					dynamic_cast<osg::Texture2D*>(texture)->setSubloadCallback(_cb);
			}

			// set dynamic data variance
			this->setDataVariance(osg::Object::DYNAMIC);
			texture->setDataVariance(osg::Object::DYNAMIC);

			this->getOrCreateStateSet()->setMode(GL_LIGHTING, 
						osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);	

			this->getOrCreateStateSet()->setTextureAttributeAndModes(0, 
						texture, osg::StateAttribute::ON);

		}
	}

	/*virtual */
	void 
	VideoGeode::setTextureMode(TextureMode textureMode)
	{
		osg::notify() << "Not implemented" << std::endl;
	}

	VideoGeode::~VideoGeode()
	{
	}

	/*virtual*/
	bool VideoGeode::addDrawable(osg::Drawable *drawable)
	{
		bool ret = Geode::addDrawable(drawable);

		if (ret) 
			this->computeTextures();

		return ret;
	}
}
