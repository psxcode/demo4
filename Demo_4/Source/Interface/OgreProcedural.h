#pragma once

#include <OgreSceneManager.h>
#include <OgreManualObject.h>

namespace Demo {
namespace OgreProcedural {
//------------------------------------------------------------
class Utils
{
public:
	static Ogre::Vector3 min(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
	{
		return Ogre::Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}
	static Ogre::Vector3 max(const Ogre::Vector3& v1, const Ogre::Vector3& v2)
	{
		return Ogre::Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}
	static void updateAABB(Ogre::AxisAlignedBox& aabb, const Ogre::AxisAlignedBox& newAABB)
	{
		aabb.setMinimum(min(aabb.getMinimum(), newAABB.getMinimum()));
		aabb.setMaximum(max(aabb.getMaximum(), newAABB.getMaximum()));
	}
	static Ogre::Vector3 vectorPermute(const Ogre::Vector3 &in)
	{
		return Ogre::Vector3(in.z, in.x, in.y);
	}

	static Ogre::Vector3 vectorAntiPermute(const Ogre::Vector3 &in)
	{
		return Ogre::Vector3(in.y, in.z, in.x);
	}
};
//------------------------------------------------------------
template <typename T>
class MeshGenerator
{
public:
	MeshGenerator( Ogre::SceneManager *osm )
		: sceneMgr(osm)
		, uTile(1.f)
		, vTile(1.f)
		, enableNormals(true)
		, numTexCoordSet(1)
	{
		_ASSERT(sceneMgr);
	}
	virtual ~MeshGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb)=0;

	inline T& setUTile(Ogre::Real uTile)	{
		this->uTile = uTile;
		return static_cast<T&>(*this);
	}
	inline T & setVTile(Ogre::Real vTile) {
		this->vTile = vTile;
		return static_cast<T&>(*this);
	}
	inline T & setEnableNormals(bool enableNormals)	{
		this->enableNormals = enableNormals;
		return static_cast<T&>(*this);
	}
	inline T & setNumTexCoordSet(unsigned int numTexCoordSet) {
		this->numTexCoordSet = numTexCoordSet;
		return static_cast<T&>(*this);
	}
	Ogre::MeshPtr realizeMesh(string_traits::in name)
	{
		Ogre::ManualObject *manual = sceneMgr->createManualObject(name);
		manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

		int offset=0;
		Ogre::AxisAlignedBox aabb;
		Ogre::Real radius(0.f);
		addToManualObject(manual, offset, radius, aabb);

		manual->end();
		Ogre::MeshPtr mesh = manual->convertToMesh(name);

		mesh->_setBounds( aabb, false );
		mesh->_setBoundingSphereRadius(radius);

		unsigned short src, dest;
		if(!mesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		{
			mesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);
		}

		return mesh;
	}

protected:
	Ogre::SceneManager* sceneMgr;
	Ogre::Real uTile;
	Ogre::Real vTile;

	bool enableNormals;
	unsigned int numTexCoordSet;
};
//------------------------------------------------------------
class PlaneGenerator : public MeshGenerator<PlaneGenerator>
{
public:
	PlaneGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<PlaneGenerator>(osm)
		, numSegX(1)
		, numSegY(1)
		, normal(Ogre::Vector3::UNIT_Y)
		, sizeX(1)
		, sizeY(1)
		, position(Ogre::Vector3::ZERO)
	{
	}
	virtual ~PlaneGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline PlaneGenerator & setNumSegX(unsigned int numSegX) {
		this->numSegX = numSegX;
		return *this;
	}
	inline PlaneGenerator & setNumSegY(unsigned int numSegY) {
		this->numSegY = numSegY;
		return *this;
	}
	inline PlaneGenerator & setNormal(Ogre::Vector3 normal)	{
		this->normal = normal;
		return *this;
	}
	inline PlaneGenerator & setSizeX(Ogre::Real sizeX) {
		this->sizeX = sizeX;
		return *this;
	}
	inline PlaneGenerator & setSizeY(Ogre::Real sizeY) {
		this->sizeY = sizeY;
		return *this;
	}
	inline PlaneGenerator & setPosition(Ogre::Vector3 position)	{
		this->position = position;
		return *this;
	}

protected:
	unsigned int numSegX;
	unsigned int numSegY;
	Ogre::Vector3 normal;
	Ogre::Real sizeX;
	Ogre::Real sizeY;
	Ogre::Vector3 position;
};
//------------------------------------------------------------
class BoxGenerator : public MeshGenerator<BoxGenerator>
{
public:
	BoxGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<BoxGenerator>(osm)
		, sizeX(1.f)
		, sizeY(1.f)
		, sizeZ(1.f)
		, numSegX(1)
		, numSegY(1)
		, numSegZ(1) 
	{
	}
	virtual ~BoxGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	BoxGenerator& setSizeX(Ogre::Real sizeX) {
		this->sizeX = sizeX;
		return *this;
	}
	BoxGenerator& setSizeY(Ogre::Real sizeY)	{
		this->sizeY = sizeY;
		return *this;
	}
	BoxGenerator& setSizeZ(Ogre::Real sizeZ) {
		this->sizeZ = sizeZ;
		return *this;
	}
	BoxGenerator& setNumSegX(unsigned int numSegX) {
		this->numSegX = numSegX;
		return *this;
	}
	BoxGenerator& setNumSegY(unsigned int numSegY) {
		this->numSegY = numSegY;
		return *this;
	}
	BoxGenerator& setNumSegZ(unsigned int numSegZ) {
		this->numSegZ = numSegZ;
		return *this;
	}

protected:
	Ogre::Real sizeX, sizeY, sizeZ;
	unsigned int numSegX, numSegY, numSegZ;
};
//------------------------------------------------------------
class RoundedBoxGenerator : public MeshGenerator<RoundedBoxGenerator>
{
public:
	RoundedBoxGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<RoundedBoxGenerator>(osm)
		, sizeX(1.f)
		, sizeY(1.f)
		, sizeZ(1.f)
		, numSegX(1)
		, numSegY(1)
		, numSegZ(1)
		, chamferSize(.1f)
		, chamferNumSeg(3)
	{
	}
	virtual ~RoundedBoxGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	RoundedBoxGenerator& setSizeX(Ogre::Real sizeX) {
		this->sizeX = sizeX;
		return *this;
	}
	RoundedBoxGenerator& setSizeY(Ogre::Real sizeY) {
		this->sizeY = sizeY;
		return *this;
	}
	RoundedBoxGenerator& setSizeZ(Ogre::Real sizeZ) {
		this->sizeZ = sizeZ;
		return *this;
	}
	RoundedBoxGenerator& setNumSegX(unsigned int numSegX) {
		this->numSegX = numSegX;
		return *this;
	}
	RoundedBoxGenerator& setNumSegY(unsigned int numSegY) {
		this->numSegY = numSegY;
		return *this;
	}
	RoundedBoxGenerator& setNumSegZ(unsigned int numSegZ) {
		this->numSegZ = numSegZ;
		return *this;
	}
	RoundedBoxGenerator& setChamferSize(Ogre::Real chamferSize)	{
		this->chamferSize = chamferSize;
		return *this;
	}
	RoundedBoxGenerator& setChamferSegsNum(unsigned int segs) {
		this->chamferNumSeg = segs;
		return *this;
	}
	void _addEdge(Ogre::ManualObject* manual, int& offset, short xPos, short yPos, short zPos);
	void _addCorner(Ogre::ManualObject* manual, int& offset, bool isXPositive, bool isYPositive, bool isZPositive);

protected:
	Ogre::Real sizeX, sizeY, sizeZ;
	unsigned int numSegX, numSegY, numSegZ;
	Ogre::Real chamferSize;
	unsigned int chamferNumSeg;
};
//------------------------------------------------------------
class SphereGenerator : public MeshGenerator<SphereGenerator>
{
public:
	SphereGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<SphereGenerator>(osm)
		, radius(1.0)
		, numRings(16)
		, numSegments(16)
	{
	}
	virtual ~SphereGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline SphereGenerator& setRadius(Ogre::Real radius) {
		this->radius = radius;
		return *this;
	}
	inline SphereGenerator& setNumRings(unsigned int numRings) {
		this->numRings = numRings;
		return *this;
	}
	inline SphereGenerator & setNumSegments(unsigned int numSegments) {
		this->numSegments = numSegments;
		return *this;
	}

protected:
	Ogre::Real radius;
	unsigned int numRings;
	unsigned int numSegments;
};
//------------------------------------------------------------
class IcoSphereGenerator : public MeshGenerator<IcoSphereGenerator>
{
public:
	IcoSphereGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<IcoSphereGenerator>(osm)
		, radius(1.)
		, numIterations(2)
	{}
	virtual ~IcoSphereGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline IcoSphereGenerator& setRadius(Ogre::Real radius)	{
		this->radius = radius;
		return *this;
	}
	inline IcoSphereGenerator& setNumIterations(unsigned int numIterations)	{
		this->numIterations = numIterations;
		return *this;
	}

protected:
	Ogre::Real radius;
	unsigned int numIterations;
};
//------------------------------------------------------------
class CylinderGenerator : public MeshGenerator<CylinderGenerator>
{
public:
	CylinderGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<CylinderGenerator>(osm)
		, numSegBase(16)
		, numSegHeight(1)
		, capped(true)
		, radius(1.f)
		, height(1.f)
	{}
	virtual ~CylinderGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline CylinderGenerator & setNumSegBase(unsigned int numSegBase) {
		this->numSegBase = numSegBase;
		return *this;
	}
	inline CylinderGenerator & setNumSegHeight(unsigned int numSegHeight) {
		this->numSegHeight = numSegHeight;
		return *this;
	}
	inline CylinderGenerator & setCapped(bool capped) {
		this->capped = capped;
		return *this;
	}
	inline CylinderGenerator & setRadius(Ogre::Real radius) {
		this->radius = radius;
		return *this;
	}
	inline CylinderGenerator & setHeight(Ogre::Real height) {
		this->height = height;
		return *this;
	}

protected:
	unsigned int numSegBase;
	unsigned int numSegHeight;
	bool capped;
	Ogre::Real radius;
	Ogre::Real height;
};
//------------------------------------------------------------
class ConeGenerator : public MeshGenerator<ConeGenerator>
{
public:
	ConeGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<ConeGenerator>(osm)
		, numSegBase(16)
		, numSegHeight(1)
		, radius(1.f)
		, height(1.f)
	{
	}
	virtual ~ConeGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline ConeGenerator & setNumSegBase(unsigned int numSegBase) {
		this->numSegBase = numSegBase;
		return *this;
	}
	inline ConeGenerator & setNumSegHeight(unsigned int numSegHeight) {
		this->numSegHeight = numSegHeight;
		return *this;
	}
	inline ConeGenerator & setRadius(Ogre::Real radius) {
		this->radius = radius;
		return *this;
	}
	inline ConeGenerator & setHeight(Ogre::Real height)
	{
		this->height = height;
		return *this;
	}

protected:
	unsigned int numSegBase;
	unsigned int numSegHeight;
	Ogre::Real radius;
	Ogre::Real height;
};
//------------------------------------------------------------
class TubeGenerator : public MeshGenerator<TubeGenerator>
{
public:
	TubeGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<TubeGenerator>(osm)
		, numSegBase(16)
		, numSegHeight(1)
		, outerRadius(2.f)
		, innerRadius(1.f)
		, height(1.f)
	{
	}
	virtual ~TubeGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline TubeGenerator & setNumSegBase(unsigned int numSegBase) {
		this->numSegBase = numSegBase;
		return *this;
	}
	inline TubeGenerator & setNumSegHeight(unsigned int numSegHeight) {
		this->numSegHeight = numSegHeight;
		return *this;
	}
	inline TubeGenerator & setOuterRadius(Ogre::Real outerRadius) {
		this->outerRadius = outerRadius;
		return *this;
	}
	inline TubeGenerator & setInnerRadius(Ogre::Real innerRadius) {
		this->innerRadius = innerRadius;
		return *this;
	}
	inline TubeGenerator & setHeight(Ogre::Real height) {
		this->height = height;
		return *this;
	}

protected:
	unsigned int numSegBase;
	unsigned int numSegHeight;
	Ogre::Real outerRadius;
	Ogre::Real innerRadius;
	Ogre::Real height;
};
//------------------------------------------------------------
class CapsuleGenerator : public MeshGenerator<CapsuleGenerator>
{
public:
	CapsuleGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<CapsuleGenerator>(osm)
		, radius(1.0)
		, numRings(8)
		, numSegments(16)
		, height(1.0)
		, numSegHeight(1)
	{
	}
	virtual ~CapsuleGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline CapsuleGenerator & setRadius(Ogre::Real radius) {
		this->radius = radius;
		return *this;
	}
	inline CapsuleGenerator & setNumRings(unsigned int numRings) {
		this->numRings = numRings;
		return *this;
	}
	inline CapsuleGenerator & setNumSegments(unsigned int numSegments) {
		this->numSegments = numSegments;
		return *this;
	}
	inline CapsuleGenerator & setNumSegHeight(unsigned int numSegHeight) {
		this->numSegHeight = numSegHeight;
		return *this;
	}
	inline CapsuleGenerator & setHeight(Ogre::Real height) {
		this->height = height;
		return *this;
	}

protected:
	Ogre::Real radius;
	unsigned int numRings;
	unsigned int numSegments;
	unsigned int numSegHeight;
	Ogre::Real height;
};
//------------------------------------------------------------
class TorusGenerator : public MeshGenerator<TorusGenerator>
{
public:
	TorusGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<TorusGenerator>(osm)
		, numSegSection(16)
		, numSegCircle(16)
		, radius(1.f)
		, sectionRadius(0.2f)
	{
	}
	virtual ~TorusGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline TorusGenerator & setNumSegSection(unsigned int numSegSection) {
		this->numSegSection = numSegSection;
		return *this;
	}
	inline TorusGenerator & setNumSegCircle(unsigned int numSegCircle) {
		this->numSegCircle = numSegCircle;
		return *this;
	}
	inline TorusGenerator & setRadius(Ogre::Real radius) {
		this->radius = radius;
		return *this;
	}
	inline TorusGenerator & setSectionRadius(Ogre::Real sectionRadius) {
		this->sectionRadius = sectionRadius;
		return *this;
	}

protected:
	unsigned int numSegSection;
	unsigned int numSegCircle;
	Ogre::Real radius;
	Ogre::Real sectionRadius;
};
//------------------------------------------------------------
class TorusKnotGenerator : public MeshGenerator<TorusKnotGenerator>
{
public:
	TorusKnotGenerator(Ogre::SceneManager *osm)
		: MeshGenerator<TorusKnotGenerator>(osm)
		, numSegSection(8)
		, numSegCircle(16)
		, radius(1.f)
		, sectionRadius(.2f)
		, p(2)
		,q(3)
	{
	}
	virtual ~TorusKnotGenerator() {}
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, Ogre::Real& boundingRadius, Ogre::AxisAlignedBox& aabb);

	inline TorusKnotGenerator & setNumSegSection(unsigned int numSegSection)	{
		this->numSegSection = numSegSection;
		return *this;
	}
	inline TorusKnotGenerator & setNumSegCircle(unsigned int numSegCircle) {
		this->numSegCircle = numSegCircle;
		return *this;
	}
	inline TorusKnotGenerator & setRadius(Ogre::Real radius) {
		this->radius = radius;
		return *this;
	}
	inline TorusKnotGenerator & setSectionRadius(Ogre::Real sectionRadius) {
		this->sectionRadius = sectionRadius;
		return *this;
	}
	inline TorusKnotGenerator & setP(unsigned int p) {
		this->p = p;
		return *this;
	}
	inline TorusKnotGenerator & setQ(unsigned int q) {
		this->q = q;
		return *this;
	}

protected:
	unsigned int numSegSection;
	unsigned int numSegCircle;
	Ogre::Real radius;
	Ogre::Real sectionRadius;
	unsigned int p;
	unsigned int q;
};
//------------------------------------------------------------
}
}