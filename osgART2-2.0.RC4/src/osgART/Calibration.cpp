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

#include "osgART/Calibration"

namespace osgART {

Calibration::Calibration() : osg::Referenced()
{
}

/* virtual */
void 
Calibration::setSize(int width, int height)
{
}

void 
Calibration::setSize(const osg::Image& image)
{
	setSize(image.s(),image.t());	
}
		
/*virtual*/ 
bool 
Calibration::load(const std::string& filename)
{
	return false;
}

osg::Camera* 
Calibration::createCamera() const
{
	osg::Camera* cam = new osg::Camera();
	cam->setRenderOrder(osg::Camera::NESTED_RENDER);
	cam->setProjectionMatrix(_projection);
	cam->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	cam->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

	return cam;	
}
		
osg::Geometry* 
Calibration::createUndistortionMesh(const osg::Vec2b& granularity) const
{

	return 0L;	
}

Calibration::~Calibration()
{
}


}

