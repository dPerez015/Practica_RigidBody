#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>

bool show_test_window = false;
namespace Cubo {
	extern void cubeSetup();
	extern void cleanupClothMesh();
	extern void updateCube(const glm::mat4 &transform);
	extern void drawCube();
	extern float halfW;
}


class Cube{
public:
	Cube() {
		size = Cubo::halfW * 2;
		mass = 1;
		InertiaTensorBody[0] = glm::vec3((1/12)*mass*(pow(size, 2)*2),0,0);
		InertiaTensorBody[1] = glm::vec3(0,(1 / 12)*mass*(pow(size, 2) * 2),0);
		InertiaTensorBody[2] = glm::vec3(0,0,(1 / 12)*mass*(pow(height, 2) + pow(width, 2)));

		randomPosMaxPos = vec3(5-sqrt(3*pow());
		reStart();

	}

	void reStart() {
	
	
	}
	~Cube() {
	
	}
	//dimensiones
	float size;
	float mass;

	//parametros extra
	glm::vec3 randomPosMaxPos, randomPosMinPos;
	float time;

	//fuerzas
	glm::vec3 forces[8];
	glm::vec3 totalForce;
	glm::vec3 totalTorque;
	glm::vec3 linearMomentum;
	glm::vec3 angularMomentum;

	//translacion
	glm::vec3 position;
	glm::vec3 linearSpeed;

	//rotación
	//su direccion es el eje de rotacion y su modulo la velocidad
	glm::vec3 angularSpeed;
	glm::quat rotation;
	glm::mat3 InertiaTensorBody;
	glm::mat3 InertiaTensor;
};


void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	//TODO
}
void PhysicsUpdate(float dt) {
	//TODO
}
void PhysicsCleanup() {
	//TODO
}