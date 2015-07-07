/*
 * This file is part of Zhockon Platform project.
 * 
 * Copyright (C) 2009-2011 William Oliveira de Lagos <william.lagos1@gmail.com>
 *
 * Zhockon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zhockon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zhockon.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <zhockon.h>
using namespace Zhockon;

Surface::Surface(int)
{
  
}

void Surface::whichPoints(std::vector<int>)
{
  
}


void Geometry::createParent(Group*)
{
  
}
void Geometry::createColorBind()
{
  
}
void Geometry::whichVertexMatrix(Matrix<int>*)
{
  
}
void Geometry::whichColorsMatrix(Matrix<float>*)
{
  
}
void Geometry::createPrimitiveSet(Surface*)
{
  
}


void Group::addChild(Geometry*)
{
  
}


Scene::Scene()
{
        root = new Group();
}

void Scene::createPyramid()
{
	Geometry* objGeo = (Geometry*)createObject();
	
	int Base[4] = {3,2,1,0};
	vector<int> basePyramid(Base,Base+sizeof(Base)/sizeof(int));
	whichPrimitives(GL_QUADS,basePyramid,objGeo);
	
	/*int pBase[4] = {3,2,1,0}; definePrimitives(GL_QUADS,(int*)pBase,objGeo);
	
	int pFaceOne[3] = {0,1,4}; definePrimitives(GL_TRIANGLES,(int*)pFaceOne,objGeo);
	int pFaceTwo[3] = {1,2,4}; definePrimitives(GL_TRIANGLES,(int*)pFaceTwo,objGeo);
	int pFaceThr[3] = {2,3,4}; definePrimitives(GL_TRIANGLES,(int*)pFaceThr,objGeo);
	int pFaceFou[3] = {3,0,4}; definePrimitives(GL_TRIANGLES,(int*)pFaceFou,objGeo);*/
	
	defineColors(objGeo);
}
void Scene::createCube()
{
	Geometry* objGeo = (Geometry*)createObject();
	
	/*int pBaseDown[4] = {3,2,1,0}; whichPrimitives(GL_QUADS,(int*)pBaseDown,objGeo);
	
	int pFaceOne[4] = {1,0,3,4}; whichPrimitives(GL_QUADS,(int*)pFaceOne,objGeo);
	int pFaceTwo[4] = {1,1,2,4}; whichPrimitives(GL_QUADS,(int*)pFaceTwo,objGeo);
	int pFaceThr[4] = {1,2,1,4}; whichPrimitives(GL_QUADS,(int*)pFaceThr,objGeo);
	int pFaceFou[4] = {1,3,0,4}; whichPrimitives(GL_QUADS,(int*)pFaceFou,objGeo);
	
	int pBaseUp[4] = {2,1,2,3}; whichPrimitives(GL_QUADS,(int*)pBaseUp,objGeo);*/
	
	defineColors(objGeo);
}
void* Scene::defineVertices()
{
	Matrix<int>* objVertices = new Matrix<int>(3);
	int FrontLeft[3]  = {0,0,0};
	int FrontRight[3] = {10,0,0};
	int BackRight[3]  = {10,10,0};
	int BackLeft[3]   = {0,10,0};
	int Peak[3] 	  = {5,5,10};
	objVertices->insert(FrontLeft); // front left
	objVertices->insert(FrontRight); // front right
	objVertices->insert(BackRight); // back right
	objVertices->insert(BackLeft); // back left
	objVertices->insert(Peak); // peak
	return (void*)objVertices;
}
void Scene::whichPrimitives(int format, vector<int> values, Geometry* objGeo)
{
	Surface* Face =
        new Surface(format);
	Face->whichPoints(values);
	objGeo->createPrimitiveSet(Face);
}
void* Scene::linkSceneObject(Geometry* objGeo)
{
        root->addChild(objGeo);
	return (void*)objGeo;
}
void* Scene::createObject()
{
        Geometry* objectGeometry = new Geometry();
        objectGeometry->whichVertexMatrix((Matrix<int>*)defineVertices());
	return (void*)objectGeometry;
}
void* Scene::defineColors(Geometry* objectGeo)
{
        Matrix<float>* colors = new Matrix<float>(4);
	float Red[4]   = {1.0f, 0.0f, 0.0f, 1.0f};
	float Green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
	float Blue[4]  = {0.0f, 0.0f, 1.0f, 1.0f};
	float White[4] = {1.0f, 1.0f, 1.0f, 1.0f}; 
	colors->insert(Red); //index 0 red
	colors->insert(Green); //index 1 green
	colors->insert(Blue); //index 2 blue
	colors->insert(White); //index 3 white

        objectGeo->whichColorsMatrix(colors);
	//objGeometry->whichColorIndices();
        objectGeo->createColorBind();
        //objGeometry->whichTexCoordMatrix();
}
void Scene::transformMode(Geometry* objectGeo)
{
        /*PositionAttitudeTransform* objectTwoXForm =
        new PositionAttitudeTransform();*/
        root->addChild(objectGeo);
        objectGeo->createParent(root);

	int objTwoPos[3] = {15,0,0};
        vector<int> objectTwoPosition(objTwoPos,objTwoPos+sizeof(objTwoPos)/sizeof(int));
        //objectTwoXForm->setPosition(objectTwoPosition);
}

void Scene::drawScene()
{
        Player player;
	/*viewer.setSceneData( root );
	viewer.run();*/
}
