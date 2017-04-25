#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>

//Boolean variables allow to show/hide the primitives
bool renderSphere = false;
bool renderCapsule = false;
bool renderParticles = false;
bool renderCloth = false;
bool renderCube = true;
namespace Sphere {
	extern void setupSphere(glm::vec3 pos = glm::vec3(0.f, 1.f, 0.f), float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();
}
namespace Capsule {
	extern void setupCapsule(glm::vec3 posA = glm::vec3(-3.f, 2.f, -2.f), glm::vec3 posB = glm::vec3(-4.f, 2.f, 2.f), float radius = 1.f);
	extern void cleanupCapsule();
	extern void updateCapsule(glm::vec3 posA, glm::vec3 posB, float radius = 1.f);
	extern void drawCapsule();
}
namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.05f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}
namespace ClothMesh {
	extern void setupClothMesh();
	extern void cleanupClothMesh();
	extern void updateClothMesh(float* array_data);
	extern void drawClothMesh();
}
namespace Cubo {
	extern void cubeSetup();
	extern void cleanupClothMesh();
	extern void updateCube(const glm::mat4& transform);
	extern void drawCube();
	extern int numOfTriangleRender;
}

void setupPrims() {
	//Sphere::setupSphere();
	//Capsule::setupCapsule();
	LilSpheres::setupParticles(LilSpheres::maxParticles);
	ClothMesh::setupClothMesh();
	Cubo::cubeSetup();

	glm::mat4 transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		2,4,-3,1
	};
	Cubo::updateCube(transform);
	
}
void cleanupPrims() {
	Sphere::cleanupSphere();
	Capsule::cleanupCapsule();
	LilSpheres::cleanupParticles();
	ClothMesh::cleanupClothMesh();
}

void renderPrims() {
	if (renderSphere)
		Sphere::drawSphere();
	if (renderCapsule)
		Capsule::drawCapsule();

	if (renderParticles) {
		LilSpheres::drawParticles(0, LilSpheres::maxParticles);
	}
	
	if (renderCloth)
		ClothMesh::drawClothMesh();

	if (renderCube)
		Cubo::drawCube();
}
