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
	extern int numOfTriangleRender;
}


class Cubo{
public:
	Cubo() {
	
	}
	~Cubo() {
	
	}

	glm::vec3 position;
	glm::vec3 linearSpeed;
	//su direccion es el eje de rotacion y su modulo la velocidad
	glm::vec3 angularSpeed;

	glm::vec3 TotalForce;
	


	glm::quat rotation;
	glm::mat4 InertiaTenBody;
	glm::mat4 InertiaTen;
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