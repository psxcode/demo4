#include "DemoStableHeaders.h"
#include "OgreProcedural.h"

namespace Demo {
namespace OgreProcedural {
//------------------------------------------------------------
/*virtual*/ void PlaneGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(!normal.isZeroLength() && "Normal must not be null");
	_ASSERT(sizeX > 0.f && sizeY > 0.f && "Size must be positive");

	Ogre::Vector3 vX = normal.perpendicular();
	Ogre::Vector3 vY = normal.crossProduct(vX);
	Ogre::Vector3 delta1 = sizeX / numSegX * vX;
	Ogre::Vector3 delta2 = sizeY / numSegY * vY;
	// build one corner of the square
	Ogre::Vector3 orig = -0.5f * sizeX * vX - 0.5f * sizeY * vY;

	for(unsigned int i1 = 0; i1 <= numSegX; ++i1) {
		for(unsigned int i2 = 0; i2 <= numSegY; ++i2)
		{
			manual->position(orig + delta1 * Ogre::Real(i1) + delta2 * Ogre::Real(i2) + position);
			manual->textureCoord(i1 / (Ogre::Real)numSegX * uTile, i2 / (Ogre::Real)numSegY * vTile);
			manual->normal(normal);
		}
	}

	bool reverse = false;
	if (delta1.crossProduct(delta2).dotProduct(normal) > 0)
		reverse = true;
	for(unsigned int n1 = 0; n1 < numSegX; ++n1)
	{
		for(unsigned int n2 = 0; n2 < numSegY; ++n2)
		{
			if(reverse)
			{
				manual->index(offset+0);
				manual->index(offset+(numSegY+1));
				manual->index(offset+1);
				manual->index(offset+1);
				manual->index(offset+(numSegY+1));
				manual->index(offset+(numSegY+1)+1);
			}
			else
			{
				manual->index(offset+0);
				manual->index(offset+1);
				manual->index(offset+(numSegY+1));
				manual->index(offset+1);
				manual->index(offset+(numSegY+1)+1);
				manual->index(offset+(numSegY+1));
			}
			++offset;
		}
		++offset;
	}
	offset += numSegY + 1;

	///TODO : Bounding volumes
}
//------------------------------------------------------------
/*virtual*/ void BoxGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(sizeX > 0. && sizeY > 0. && sizeZ > 0. && "Sizes must be positive");

	PlaneGenerator pg(sceneMgr);
	pg.setUTile(uTile).setVTile(vTile);
	pg.setNumSegX(numSegY).setNumSegY(numSegX).setSizeX(sizeY).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::NEGATIVE_UNIT_Z)
		.setPosition(.5f * sizeZ * Ogre::Vector3::NEGATIVE_UNIT_Z)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegY).setNumSegY(numSegX).setSizeX(sizeY).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::UNIT_Z)
		.setPosition(.5f * sizeZ * Ogre::Vector3::UNIT_Z)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegX).setSizeX(sizeZ).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::NEGATIVE_UNIT_Y)
		.setPosition(.5f * sizeY * Ogre::Vector3::NEGATIVE_UNIT_Y)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegX).setSizeX(sizeZ).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::UNIT_Y)
		.setPosition(.5f * sizeY * Ogre::Vector3::UNIT_Y)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegY).setSizeX(sizeZ).setSizeY(sizeY)
		.setNormal(Ogre::Vector3::NEGATIVE_UNIT_X)
		.setPosition(.5f * sizeX * Ogre::Vector3::NEGATIVE_UNIT_X)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegY).setSizeX(sizeZ).setSizeY(sizeY)
		.setNormal(Ogre::Vector3::UNIT_X)
		.setPosition(.5f * sizeX * Ogre::Vector3::UNIT_X)
		.addToManualObject(manual, offset, boundingRadius, aabb);

	aabb.setExtents(-.5f * sizeX, -.5f * sizeY, -.5f * sizeZ, .5f * sizeX, .5f * sizeY, .5f * sizeZ);
	boundingRadius = Ogre::Math::Sqrt(sizeX * sizeX + sizeY * sizeY + sizeZ * sizeZ);
}
//------------------------------------------------------------
/*virtual*/ void SphereGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(radius > 0 && "Radius must be positive");

	Ogre::Real fDeltaRingAngle = (Ogre::Math::PI / numRings);
	Ogre::Real fDeltaSegAngle = (Ogre::Math::TWO_PI / numSegments);

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= numRings; ++ring)
	{
		Ogre::Real r0 = radius * sinf(ring * fDeltaRingAngle);
		Ogre::Real y0 = radius * cosf(ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= numSegments; ++seg)
		{
			Ogre::Real x0 = r0 * sinf(seg * fDeltaSegAngle);
			Ogre::Real z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, y0, z0);
			if (enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for(unsigned int tc = 0; tc < numTexCoordSet; ++tc)
				manual->textureCoord((Ogre::Real)seg / (Ogre::Real)numSegments * uTile, (Ogre::Real)ring / (Ogre::Real)numRings * vTile);

			if(ring != numRings)
			{
				// each vertex (except the last) has six indices pointing to it
				manual->index(offset + numSegments + 1);
				manual->index(offset);
				manual->index(offset + numSegments);
				manual->index(offset + numSegments + 1);
				manual->index(offset + 1);
				manual->index(offset);
				++offset;
			}
		}; // end for seg
	} // end for ring

	boundingRadius = radius;
	Utils::updateAABB(aabb, Ogre::AxisAlignedBox(-radius, -radius, -radius, radius, radius, radius));
}
//------------------------------------------------------------
/*virtual*/ void IcoSphereGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(radius > 0. && "Radius must me positive");

	typedef Demo::vector<Ogre::Vector3>::type vertices_cont;
	vertices_cont vertices;

	/// Step 1 : Generate icosahedron
	Ogre::Real phi = .5f * (1.f + sqrt(5.f));
	Ogre::Real invnorm = 1 / sqrt(phi * phi + 1);

	vertices.push_back(invnorm*Ogre::Vector3(-1,  phi, 0));//0
	vertices.push_back(invnorm*Ogre::Vector3( 1,  phi, 0));//1
	vertices.push_back(invnorm*Ogre::Vector3(0,   1,  -phi));//2
	vertices.push_back(invnorm*Ogre::Vector3(0,   1,   phi));//3
	vertices.push_back(invnorm*Ogre::Vector3(-phi,0,  -1));//4
	vertices.push_back(invnorm*Ogre::Vector3(-phi,0,   1));//5
	vertices.push_back(invnorm*Ogre::Vector3( phi,0,  -1));//6
	vertices.push_back(invnorm*Ogre::Vector3( phi,0,   1));//7
	vertices.push_back(invnorm*Ogre::Vector3(0,   -1, -phi));//8
	vertices.push_back(invnorm*Ogre::Vector3(0,   -1,  phi));//9
	vertices.push_back(invnorm*Ogre::Vector3(-1,  -phi,0));//10
	vertices.push_back(invnorm*Ogre::Vector3( 1,  -phi,0));//11

	int firstFaces[] = {
		0,1,2,
		0,3,1,
		0,4,5,
		1,7,6,
		1,6,2,
		1,3,7,
		0,2,4,
		0,5,3,
		2,6,8,
		2,8,4,
		3,5,9,
		3,9,7,
		11,6,7,
		10,5,4,
		10,4,8,
		10,9,5,
		11,8,6,
		11,7,9,
		10,8,11,
		10,11,9
	};

	typedef Demo::vector<int>::type faces_cont;
	faces_cont faces(firstFaces, firstFaces + sizeof(firstFaces)/sizeof(*firstFaces));
	int size = 60;

	/// Step 2 : tessellate
	for(unsigned int iteration = 0; iteration < numIterations; ++iteration)
	{
		size *= 4;
		faces_cont newFaces;
		newFaces.clear();
		//newFaces.resize(size);
		for(int i = 0, iter_count = size / 12; i < iter_count; ++i)
		{
			int i1 = faces[i*3];
			int i2 = faces[i*3+1];
			int i3 = faces[i*3+2];
			int i12 = vertices.size();
			int i23 = i12+1;
			int i13 = i12+2;
			Ogre::Vector3 v1 = vertices[i1];
			Ogre::Vector3 v2 = vertices[i2];
			Ogre::Vector3 v3 = vertices[i3];
			//make 1 vertice at the center of each edge and project it onto the sphere
			vertices.push_back((v1+v2).normalisedCopy());
			vertices.push_back((v2+v3).normalisedCopy());
			vertices.push_back((v1+v3).normalisedCopy());
			//now recreate indices
			newFaces.push_back(i1);
			newFaces.push_back(i12);
			newFaces.push_back(i13);
			newFaces.push_back(i2);
			newFaces.push_back(i23);
			newFaces.push_back(i12);
			newFaces.push_back(i3);
			newFaces.push_back(i13);
			newFaces.push_back(i23);
			newFaces.push_back(i12);
			newFaces.push_back(i23);
			newFaces.push_back(i13);
		}
		faces.swap(newFaces);
	}

	/// Step 3 : generate texcoords
	typedef Demo::vector<Ogre::Vector2>::type texcoords_cont;
	texcoords_cont texCoords;
	for(vertices_cont::size_type i = 0, iter_count = vertices.size(); i < iter_count; ++i)
	{
		const Ogre::Vector3& vec = vertices[i];
		Ogre::Real u, v;
		Ogre::Real r0 = sqrtf(vec.x * vec.x + vec.z * vec.z);
		Ogre::Real alpha;
		alpha = atan2f(vec.z, vec.x);
		u = alpha / Ogre::Math::TWO_PI + Ogre::Real(.5);
		v = atan2f(vec.y, r0) / Ogre::Math::PI + Ogre::Real(.5);
		texCoords.push_back(Ogre::Vector2(u,v));
	}

	/// Step 4 : fix texcoords
	// find vertices to split
	typedef Demo::vector<int>::type indexToSplit_cont;
	indexToSplit_cont indexToSplit;
	for(faces_cont::size_type i = 0, iter_count = faces.size() / 3; i < iter_count; ++i)
	{
		Ogre::Vector2& t0 = texCoords[faces[i*3+0]];
		Ogre::Vector2& t1 = texCoords[faces[i*3+1]];
		Ogre::Vector2& t2 = texCoords[faces[i*3+2]];
		if(abs(t2.x - t0.x) > 0.5)
		{
			if (t0.x < 0.5)	indexToSplit.push_back(faces[i*3]);
			else indexToSplit.push_back(faces[i*3+2]);
		}
		if(abs(t1.x - t0.x) > 0.5)
		{
			if (t0.x < 0.5) indexToSplit.push_back(faces[i*3]);
			else indexToSplit.push_back(faces[i*3+1]);
		}
		if(abs(t2.x - t1.x) > 0.5)
		{
			if (t1.x < 0.5) indexToSplit.push_back(faces[i*3+1]);
			else indexToSplit.push_back(faces[i*3+2]);
		}
	}
	//split vertices
	for(indexToSplit_cont::size_type i = 0, i_count = indexToSplit.size(); i < i_count; ++i)
	{
		int index = indexToSplit[i];
		//duplicate vertex
		Ogre::Vector3 v = vertices[index];
		Ogre::Vector2 t = texCoords[index] + Ogre::Vector2::UNIT_X;
		vertices.push_back(v);
		texCoords.push_back(t);
		int newIndex = vertices.size()-1;
		//reassign indices
		for(faces_cont::size_type j = 0, j_count = faces.size(); j < j_count; ++j)
		{
			if(faces[j] == index)
			{
				int index1 = faces[(j+1)%3+(j/3)*3];
				int index2 = faces[(j+2)%3+(j/3)*3];
				if ((texCoords[index1].x > 0.5) || (texCoords[index2].x > 0.5))
				{
					faces[j] = newIndex;
				}
			}
		}
	}

	/// Step 5 : realize
	for(vertices_cont::size_type i = 0, i_count = vertices.size(); i < i_count; ++i)
	{
		manual->position(radius*vertices[i]);
		if (enableNormals)
			manual->normal(vertices[i]);//note : vertices are already normalised
		for(unsigned int tc = 0; tc < numTexCoordSet; ++tc)
			manual->textureCoord(texCoords[i].x*uTile,texCoords[i].y*vTile);
	}
	for(int i = 0; i < size; ++i)
	{
		manual->index(offset+faces[i]);
	}
	offset += vertices.size();
	boundingRadius = radius;
	Utils::updateAABB(aabb, Ogre::AxisAlignedBox(-radius, -radius, -radius, radius, radius, radius));
}
//------------------------------------------------------------
/*virtual*/ void RoundedBoxGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	// Generate the pseudo-box shape
	PlaneGenerator pg(sceneMgr);
	pg.setUTile(uTile).setVTile(vTile);
	pg.setNumSegX(numSegY).setNumSegY(numSegX).setSizeX(sizeY).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::NEGATIVE_UNIT_Z)
		.setPosition((.5f * sizeZ + chamferSize) * Ogre::Vector3::NEGATIVE_UNIT_Z)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegY).setNumSegY(numSegX).setSizeX(sizeY).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::UNIT_Z)
		.setPosition((.5f * sizeZ + chamferSize) * Ogre::Vector3::UNIT_Z)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegX).setSizeX(sizeZ).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::NEGATIVE_UNIT_Y)
		.setPosition((.5f * sizeY + chamferSize) * Ogre::Vector3::NEGATIVE_UNIT_Y)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegX).setSizeX(sizeZ).setSizeY(sizeX)
		.setNormal(Ogre::Vector3::UNIT_Y)
		.setPosition((.5f * sizeY + chamferSize) * Ogre::Vector3::UNIT_Y)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegY).setSizeX(sizeZ).setSizeY(sizeY)
		.setNormal(Ogre::Vector3::NEGATIVE_UNIT_X)
		.setPosition((.5f * sizeX + chamferSize) * Ogre::Vector3::NEGATIVE_UNIT_X)
		.addToManualObject(manual, offset, boundingRadius, aabb);
	pg.setNumSegX(numSegZ).setNumSegY(numSegY).setSizeX(sizeZ).setSizeY(sizeY)
		.setNormal(Ogre::Vector3::UNIT_X)
		.setPosition((.5f * sizeX + chamferSize) * Ogre::Vector3::UNIT_X)
		.addToManualObject(manual, offset, boundingRadius, aabb);

	// Generate the corners
	_addCorner(manual, offset, true,  true,  true);
	_addCorner(manual, offset, true,  true,  false);
	_addCorner(manual, offset, true,  false, true);
	_addCorner(manual, offset, true,  false, false);
	_addCorner(manual, offset, false, true,  true);
	_addCorner(manual, offset, false, true,  false);
	_addCorner(manual, offset, false, false, true);
	_addCorner(manual, offset, false, false, false);

	// Generate the edges
	_addEdge(manual, offset, -1,-1, 0);
	_addEdge(manual, offset, -1, 1, 0);
	_addEdge(manual, offset,  1,-1, 0);
	_addEdge(manual, offset,  1, 1, 0);
	_addEdge(manual, offset, -1, 0,-1);
	_addEdge(manual, offset, -1, 0, 1);
	_addEdge(manual, offset,  1, 0,-1);
	_addEdge(manual, offset,  1, 0, 1);	
	_addEdge(manual, offset,  0,-1,-1);
	_addEdge(manual, offset,  0,-1, 1);
	_addEdge(manual, offset,  0, 1,-1);
	_addEdge(manual, offset,  0, 1, 1);

	aabb.setExtents(-.5f * sizeX, -.5f * sizeY, -.5f * sizeZ, .5f * sizeX, .5f * sizeY, .5f * sizeZ);
	boundingRadius = Ogre::Math::Sqrt(sizeX*sizeX + sizeY*sizeY + sizeZ*sizeZ);
}
//------------------------------------------------------------
/*virtual*/ void RoundedBoxGenerator::_addEdge(Ogre::ManualObject* manual, int& offset, short xPos, short yPos, short zPos)
{
	Ogre::Vector3 centerPosition = .5f * xPos * sizeX * Ogre::Vector3::UNIT_X + .5f * yPos * sizeY * Ogre::Vector3::UNIT_Y + .5f * zPos * sizeZ * Ogre::Vector3::UNIT_Z;
	Ogre::Vector3 vy0 = Ogre::Real(1-abs(xPos)) * Ogre::Vector3::UNIT_X + Ogre::Real(1-abs(yPos)) * Ogre::Vector3::UNIT_Y + Ogre::Real(1-abs(zPos)) * Ogre::Vector3::UNIT_Z;//extrusion direction	

	Ogre::Vector3 vx0 = Utils::vectorAntiPermute(vy0);
	Ogre::Vector3 vz0 = Utils::vectorPermute(vy0);
	if (vx0.dotProduct(centerPosition)<0.0) vx0=-vx0;
	if (vz0.dotProduct(centerPosition)<0.0) vz0=-vz0;
	if (vx0.crossProduct(vy0).dotProduct(vz0)<0.0) vy0=-vy0;

	Ogre::Real height = (1-abs(xPos)) * sizeX + (1-abs(yPos)) * sizeY + (1-abs(zPos)) * sizeZ;//TODO
	Ogre::Vector3 offsetPosition = centerPosition - .5f * height * vy0;
	unsigned int numSegHeight=1;//TODO

	Ogre::Real deltaAngle = (Ogre::Math::HALF_PI / chamferNumSeg);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	if (xPos==0)
		numSegHeight = numSegX;
	else if (yPos==0)
		numSegHeight = numSegY;
	else if (zPos==0)
		numSegHeight = numSegZ;

	for(unsigned int i = 0; i <= numSegHeight; ++i)
		for(unsigned int j = 0; j <= chamferNumSeg; ++j)
		{
			Ogre::Real x0 = chamferSize * cosf(j*deltaAngle);
			Ogre::Real z0 = chamferSize * sinf(j*deltaAngle);
			manual->position(x0 * vx0 + i*deltaHeight * vy0 + z0 * vz0 + offsetPosition);
			manual->normal((x0*vx0+z0*vz0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)chamferNumSeg*uTile, i/(Ogre::Real)numSegHeight*vTile);

			if (i != numSegHeight && j!=chamferNumSeg) {
				manual->index(offset + chamferNumSeg + 2);
				manual->index(offset);
				manual->index(offset + chamferNumSeg+1);
				manual->index(offset + chamferNumSeg + 2);
				manual->index(offset + 1);
				manual->index(offset);
			}
			offset ++;
		}
}
//------------------------------------------------------------
/*virtual*/ void RoundedBoxGenerator::_addCorner(Ogre::ManualObject* manual, int& offset, bool isXPositive, bool isYPositive, bool isZPositive)
{
	_ASSERT(sizeX > 0.f && sizeY > 0.f && sizeZ > 0.f && chamferSize > 0.f && "Sizes must be positive");

	Ogre::Vector3 offsetPosition((isXPositive ? 1 : -1) * .5f * sizeX, (isYPositive ? 1 : -1) * .5f * sizeY, (isZPositive ? 1 : -1) * .5f * sizeZ);
	Ogre::Real deltaRingAngle = (Ogre::Math::HALF_PI / chamferNumSeg);
	Ogre::Real deltaSegAngle = (Ogre::Math::HALF_PI / chamferNumSeg);
	Ogre::Real offsetRingAngle = isYPositive?0:Ogre::Math::HALF_PI;
	Ogre::Real offsetSegAngle;
	if (isXPositive&&isZPositive) offsetSegAngle = 0;
	if ((!isXPositive)&&isZPositive) offsetSegAngle = 1.5f * Ogre::Math::PI;
	if (isXPositive&&(!isZPositive)) offsetSegAngle = Ogre::Math::HALF_PI;
	if ((!isXPositive)&&(!isZPositive)) offsetSegAngle = Ogre::Math::PI;

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= chamferNumSeg; ++ring )
	{
		Ogre::Real r0 = chamferSize * sinf (ring * deltaRingAngle + offsetRingAngle);
		Ogre::Real y0 = chamferSize * cosf (ring * deltaRingAngle + offsetRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= chamferNumSeg; seg++)
		{
			Ogre::Real x0 = r0 * sinf(seg * deltaSegAngle + offsetSegAngle);
			Ogre::Real z0 = r0 * cosf(seg * deltaSegAngle + offsetSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0 + offsetPosition.x, y0 + offsetPosition.y, z0 + offsetPosition.z);
			if (enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for (unsigned int tc = 0; tc < numTexCoordSet; ++tc)
				manual->textureCoord((Ogre::Real) seg / (Ogre::Real) chamferNumSeg * uTile, (Ogre::Real) ring / (Ogre::Real) chamferNumSeg * vTile);

			if ((ring != chamferNumSeg) && (seg != chamferNumSeg))
			{
				//if (ring != chamferNumSeg) {
				// each vertex (except the last) has six indices pointing to it
				manual->index(offset + chamferNumSeg + 2);
				manual->index(offset);
				manual->index(offset + chamferNumSeg +1);
				manual->index(offset + chamferNumSeg + 2);
				manual->index(offset + 1);
				manual->index(offset);
			}
			++offset;
		} // end for seg
	} // end for ring
}
//------------------------------------------------------------
/*virtual*/ void CylinderGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(height > 0.f && radius > 0.f && "Height and radius must be positive");

	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	for(unsigned int i = 0; i <= numSegHeight; ++i) {
		for(unsigned int j = 0; j <= numSegBase; ++j)
		{
			Ogre::Real x0 = radius * cosf(j*deltaAngle);
			Ogre::Real z0 = radius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);

			if (i != numSegHeight) {
				manual->index(offset + numSegBase + 1);
				manual->index(offset);
				manual->index(offset + numSegBase);
				manual->index(offset + numSegBase + 1);
				manual->index(offset + 1);
				manual->index(offset);
			}
			++offset;
		}
	}

	if(capped)
	{
		//low cap
		int centerIndex = offset;
		manual->position(0,0,0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(0.0,vTile);
		++offset;
		for(unsigned int j = 0; j <= numSegBase; ++j)
		{
			Ogre::Real x0 = radius * cosf(j*deltaAngle);
			Ogre::Real z0 = radius * sinf(j*deltaAngle);

			manual->position(x0, 0.0f, z0);
			manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase,0.0);
			if(j != numSegBase)
			{
				manual->index(centerIndex);
				manual->index(offset);
				manual->index(offset+1);
			}
			++offset;
		}
		// high cap
		centerIndex = offset;
		manual->position(0,height,0);
		manual->normal(Ogre::Vector3::UNIT_Y);
		manual->textureCoord(0.0,0.0);
		offset++;
		for(unsigned int j = 0; j <= numSegBase; ++j)
		{
			Ogre::Real x0 = radius * cosf(j*deltaAngle);
			Ogre::Real z0 = radius * sinf(j*deltaAngle);

			manual->position(x0, height, z0);
			manual->normal(Ogre::Vector3::UNIT_Y);
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,vTile);
			if( j != numSegBase)
			{
				manual->index(centerIndex);
				manual->index(offset+1);
				manual->index(offset);
			}
			++offset;
		}
	}

	boundingRadius = std::max(radius, height);
	aabb = Ogre::AxisAlignedBox(-radius,0,-radius,radius, height, radius);
}
//------------------------------------------------------------
/*virtual*/ void ConeGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(height > 0.f && radius > 0.f && "Height and radius must be positive");
	
	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	Ogre::Vector3 refNormal = Ogre::Vector3(radius, height, 0.f).normalisedCopy();
	Ogre::Quaternion q;

	for(unsigned int i = 0; i <= numSegHeight; ++i)
	{
		Ogre::Real r0 = radius * (1 - i / (Ogre::Real)numSegHeight);
		for(unsigned int j = 0; j <= numSegBase; ++j)
		{
			Ogre::Real x0 = r0* cosf(j*deltaAngle);
			Ogre::Real z0 = r0 * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			q.FromAngleAxis(Ogre::Radian(-deltaAngle * j), Ogre::Vector3::UNIT_Y);
			manual->normal(q*refNormal);
			//if (i != numSegHeight)
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);
			/*else
			manual->textureCoord(0.f, i/(Ogre::Real)numSegHeight*vTile);*/

			if (i != numSegHeight&& j != numSegBase)
			{
				manual->index(offset + numSegBase + 2);
				manual->index(offset);
				manual->index(offset + numSegBase+1);
				manual->index(offset + numSegBase + +2);
				manual->index(offset + 1);
				manual->index(offset);
			}
			++offset;
		}
	}

	//low cap
	int centerIndex = offset;
	manual->position(0,0,0);
	manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
	manual->textureCoord(0.0,vTile);
	offset++;
	for(unsigned int j = 0; j <= numSegBase; ++j)
	{
		Ogre::Real x0 = radius * cosf(j*deltaAngle);
		Ogre::Real z0 = radius * sinf(j*deltaAngle);

		manual->position(x0, 0.0f, z0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);
		if(j != numSegBase)
		{
			manual->index(centerIndex);
			manual->index(offset);
			manual->index(offset+1);
		}
		++offset;
	}

/*    boundingRadius = Utils::max(radius, height);
	aabb = Ogre::AxisAlignedBox(-radius,0,-radius,radius, height, radius);*/
}
//------------------------------------------------------------
/*virtual*/ void CapsuleGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(height > 0.f && radius > 0.f && "Height and radius must be positive");

	Ogre::Real fDeltaRingAngle = (Ogre::Math::HALF_PI / numRings);
	Ogre::Real fDeltaSegAngle = (Ogre::Math::TWO_PI / numSegments);

	float sphereRatio = radius / (2 * radius + height);
	float cylinderRatio = height / (2 * radius + height);
	// Top half sphere

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= numRings; ++ring)
	{
		Ogre::Real r0 = radius * sinf ( ring * fDeltaRingAngle);
		Ogre::Real y0 = radius * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= numSegments; ++seg)
		{
			Ogre::Real x0 = r0 * cosf(seg * fDeltaSegAngle);
			Ogre::Real z0 = r0 * sinf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, .5f * height + y0, z0);
			if(enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for(unsigned int tc = 0; tc < numTexCoordSet; ++tc)
				manual->textureCoord((Ogre::Real) seg / (Ogre::Real) numSegments * uTile, (Ogre::Real) ring / (Ogre::Real) numRings * vTile * sphereRatio);

			//if (ring != numRings) {
			// each vertex (except the last) has six indices pointing to it
			manual->index(offset + numSegments + 1);
			manual->index(offset + numSegments);
			manual->index(offset);
			manual->index(offset + numSegments + 1);
			manual->index(offset);
			manual->index(offset + 1);
			//}
			++offset;
		}; // end for seg
	} // end for ring

	// Cylinder part
	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegments);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	for(unsigned int i = 1; i <= numSegHeight - 1; ++i)
	{
		for(unsigned int j = 0; j <= numSegments; ++j)
		{
			Ogre::Real x0 = radius * cosf(j*deltaAngle);
			Ogre::Real z0 = radius * sinf(j*deltaAngle);
			manual->position(x0, .5f * height - i * deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegments*uTile, i/(Ogre::Real)numSegHeight*vTile * cylinderRatio + sphereRatio);

			//if (i != numSegHeight) {
			manual->index(offset + numSegments + 1);
			manual->index(offset + numSegments);
			manual->index(offset);
			manual->index(offset + numSegments + 1);
			manual->index(offset);
			manual->index(offset + 1);
			//}
			++offset;
		}
	}

	// Bottom half sphere

	// Generate the group of rings for the sphere
	for(unsigned int ring = 0; ring <= numRings; ++ring)
	{
		Ogre::Real r0 = radius * sinf (Ogre::Math::HALF_PI + ring * fDeltaRingAngle);
		Ogre::Real y0 =  radius * cosf (Ogre::Math::HALF_PI + ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(unsigned int seg = 0; seg <= numSegments; ++seg)
		{
			Ogre::Real x0 = r0 * cosf(seg * fDeltaSegAngle);
			Ogre::Real z0 = r0 * sinf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			manual->position( x0, -.5f * height + y0, z0);
			if (enableNormals)
				manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			for (unsigned int tc = 0; tc < numTexCoordSet; ++tc)
				manual->textureCoord((Ogre::Real) seg / (Ogre::Real) numSegments * uTile, (Ogre::Real) ring / (Ogre::Real) numRings * vTile*sphereRatio + cylinderRatio + sphereRatio);

			if (ring != numRings) {
				// each vertex (except the last) has six indices pointing to it
				manual->index(offset + numSegments + 1);
				manual->index(offset + numSegments);
				manual->index(offset);
				manual->index(offset + numSegments + 1);
				manual->index(offset);
				manual->index(offset + 1);
			}
			++offset;
		}; // end for seg
	} // end for ring

	boundingRadius = height + radius;
	Utils::updateAABB(aabb, Ogre::AxisAlignedBox(-radius, -radius-height, -radius, radius, radius+height, radius));
}
//------------------------------------------------------------
/*virtual*/ void TorusGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(radius>0. && sectionRadius>0. && "Radius must be positive");

	Ogre::Real deltaSection = (Ogre::Math::TWO_PI / numSegSection);
	Ogre::Real deltaCircle = (Ogre::Math::TWO_PI / numSegCircle);

	for(unsigned int i = 0; i <= numSegCircle; ++i)
	{
		for(unsigned int j = 0; j <= numSegSection; ++j)
		{
			Ogre::Vector3 c0(radius, 0.0, 0.0);
			Ogre::Vector3 v0(radius+sectionRadius * cosf(j*deltaSection),sectionRadius * sinf(j*deltaSection),0.0);
			Ogre::Quaternion q;
			q.FromAngleAxis(Ogre::Radian(i*deltaCircle),Ogre::Vector3::UNIT_Y);
			Ogre::Vector3 v = q * v0;
			Ogre::Vector3 c = q * c0;
			manual->position(v);
			manual->normal((v-c).normalisedCopy());
			manual->textureCoord(i/(Ogre::Real)numSegCircle*uTile, j/(Ogre::Real)numSegSection*vTile);

			if(i != numSegCircle)
			{
				manual->index(offset + numSegSection + 1);
				manual->index(offset);
				manual->index(offset + numSegSection);
				manual->index(offset + numSegSection + 1);
				manual->index(offset + 1);
				manual->index(offset);
			}
			++offset;
		}
	}

	boundingRadius = radius + sectionRadius;
	aabb = Ogre::AxisAlignedBox(-radius-sectionRadius,-sectionRadius,-radius-sectionRadius, radius+sectionRadius, sectionRadius, radius+sectionRadius);
}
//------------------------------------------------------------
/*virtual*/ void TorusKnotGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(radius > 0.f && sectionRadius > 0.f && "Radius must be positive");

	for(unsigned int i = 0; i <= numSegCircle * p; ++i)
	{
		Ogre::Real phi = Ogre::Math::TWO_PI * i/(Ogre::Real)numSegCircle;
		Ogre::Real x0 = radius*(2 + cos(q*phi/(Ogre::Real)p)) * cos(phi) / 3.f;
		Ogre::Real y0 = radius*sin(q*phi/(Ogre::Real)p) / 3.f;
		Ogre::Real z0 = radius*(2 + cos(q*phi/(Ogre::Real)p)) * sin(phi) / 3.f;

		Ogre::Real phi1 = Ogre::Math::TWO_PI * (i+1)/(Ogre::Real)numSegCircle;
		Ogre::Real x1 = radius*(2 + cos(q*phi1/p)) * cos(phi1) / 3.f;
		Ogre::Real y1 = radius*sin(q*phi1/p) / 3.f;
		Ogre::Real z1 = radius*(2 + cos(q*phi1/p)) * sin(phi1) / 3.f;

		Ogre::Vector3 v0(x0,y0,z0);
		Ogre::Vector3 v1(x1,y1,z1);
		Ogre::Vector3 direction((v1-v0).normalisedCopy());

		// First, compute an approximate quaternion (everything is ok except Roll angle)
		Ogre::Quaternion quat = Ogre::Vector3::UNIT_Z.getRotationTo(direction);
		// Then, compute a correction quaternion : we want the "up" direction to be always the same
		Ogre::Vector3 projectedY = Ogre::Vector3::UNIT_Y - Ogre::Vector3::UNIT_Y.dotProduct(direction) * direction;		
		Ogre::Vector3 tY = quat * Ogre::Vector3::UNIT_Y;
		Ogre::Quaternion quat2 = tY.getRotationTo(projectedY);
		Ogre::Quaternion q = quat2 * quat;

		for(unsigned int j = 0; j <= numSegSection; ++j)
		{
			Ogre::Real alpha = Ogre::Math::TWO_PI *j/numSegSection;
			Ogre::Vector3 vp = sectionRadius*(q * Ogre::Vector3(cos(alpha), sin(alpha),0));

			manual->position(v0+vp);
			manual->normal(vp.normalisedCopy());
			manual->textureCoord(i/(Ogre::Real)numSegCircle*uTile, j/(Ogre::Real)numSegSection*vTile);

			if(i != numSegCircle * p)
			{
				manual->index(offset + numSegSection + 1);
				manual->index(offset + numSegSection);
				manual->index(offset);
				manual->index(offset + numSegSection + 1);
				manual->index(offset);
				manual->index(offset + 1);
			}
			++offset;
		}
	}

	boundingRadius = Ogre::Math::Sqrt(2*Ogre::Math::Sqr(radius + sectionRadius)+Ogre::Math::Sqr(sectionRadius));
	aabb = Ogre::AxisAlignedBox(-radius-sectionRadius,-sectionRadius,-radius-sectionRadius, radius+sectionRadius, sectionRadius, radius+sectionRadius);
}
//------------------------------------------------------------
/*virtual*/ void TubeGenerator::addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)
{
	_ASSERT(height > 0.f && outerRadius > 0.f && innerRadius > 0.f && "Height and radius must be positive");
	_ASSERT(innerRadius < outerRadius && "Outer radius must be bigger than inner radius");

	Ogre::Real deltaAngle = (Ogre::Math::TWO_PI / numSegBase);
	Ogre::Real deltaHeight = height/(Ogre::Real)numSegHeight;

	for(unsigned int i = 0; i <= numSegHeight; ++i)
		for(unsigned int j = 0; j <= numSegBase; ++j)
		{
			Ogre::Real x0 = outerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = outerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);

			if(i != numSegHeight)
			{
				manual->index(offset + numSegBase + 1);
				manual->index(offset);
				manual->index(offset + numSegBase);
				manual->index(offset + numSegBase + 1);
				manual->index(offset + 1);
				manual->index(offset);
			}
			++offset;
		}

	for(unsigned int i = 0; i <= numSegHeight; ++i)
		for(unsigned int j = 0; j <= numSegBase; ++j)
		{
			Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
			Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);
			manual->position(x0, i*deltaHeight, z0);
			manual->normal(-Ogre::Vector3(x0,0,z0).normalisedCopy());
			manual->textureCoord(j/(Ogre::Real)numSegBase*uTile, i/(Ogre::Real)numSegHeight*vTile);

			if(i != numSegHeight)
			{
				manual->index(offset + numSegBase + 1);
				manual->index(offset + numSegBase);
				manual->index(offset);
				manual->index(offset + numSegBase + 1);
				manual->index(offset);
				manual->index(offset + 1);
			}
			++offset;
		}

	//low cap
	for(unsigned int j = 0; j <= numSegBase; ++j)
	{
		Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
		Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);

		manual->position(x0, 0.0f, z0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,vTile);

		x0 = outerRadius * cosf(j*deltaAngle);
		z0 = outerRadius * sinf(j*deltaAngle);

		manual->position(x0, 0.0f, z0);
		manual->normal(Ogre::Vector3::NEGATIVE_UNIT_Y);
		manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);

		if(j!=numSegBase)
		{
			manual->index(offset);
			manual->index(offset+1);
			manual->index(offset+3);
			manual->index(offset+2);
			manual->index(offset);
			manual->index(offset+3);
		}
		offset += 2;
	}

	//high cap
	for(unsigned int j = 0; j <= numSegBase; ++j)
	{
		Ogre::Real x0 = innerRadius * cosf(j*deltaAngle);
		Ogre::Real z0 = innerRadius * sinf(j*deltaAngle);

		manual->position(x0, height, z0);
		manual->normal(Ogre::Vector3::UNIT_Y);
		manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,0.0);

		x0 = outerRadius * cosf(j*deltaAngle);
		z0 = outerRadius * sinf(j*deltaAngle);

		manual->position(x0, height, z0);
		manual->normal(Ogre::Vector3::UNIT_Y);
		manual->textureCoord(j/(Ogre::Real)numSegBase*uTile,vTile);

		if(j!=numSegBase)
		{
			manual->index(offset+1);
			manual->index(offset);
			manual->index(offset+3);
			manual->index(offset);
			manual->index(offset+2);
			manual->index(offset+3);
		}
		offset += 2;
	}

	boundingRadius = std::max(outerRadius, height);
	aabb = Ogre::AxisAlignedBox(-outerRadius,0,-outerRadius,outerRadius, height, outerRadius);
}
//------------------------------------------------------------
}
}