#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>

bool show_test_window = false;
namespace Cubo {
	extern void cubeSetup(int num);
	extern void cleanupClothMesh();
	extern void updateCube(float* array_data);
	extern void drawCube();
	extern int numOfTriangleRender;
}

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::DragInt("numero de triangulos", &Cubo::numOfTriangleRender, 0.05,3,14);
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