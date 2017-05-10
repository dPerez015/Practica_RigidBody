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
glm::vec3 gravity(0,-9.8,0);

class Cube{
public:
	Cube() {
		//datos
		size = Cubo::halfW * 2;
		mass = 1;
		//tensor de incercia inicial (Ibody)
		InertiaTensorBody[0] = glm::vec3((1/12)*mass*(pow(size, 2)*2),0,0);
		InertiaTensorBody[1] = glm::vec3(0,(1 / 12)*mass*(pow(size, 2) * 2),0);
		InertiaTensorBody[2] = glm::vec3(0,0,(1 / 12)*mass*(pow(size, 2) + pow(size, 2)));

		InertiaTensorBody = glm::inverse(InertiaTensorBody);

		//maximo tamaño de la fuerza
		MaxForceApplied = 1000;

		maxTime = 5;
		time = maxTime + 1;

	}
	~Cube() {

	}
	void reStart() {
		//inicializar parametros
		time = 0;

		//inicializamos posicion
		position.x = ((rand() / RAND_MAX) - 0.5) *(5-size);
		position.y = (rand() / RAND_MAX)*5+size/2;
		position.z = ((rand() / RAND_MAX)*5)-size/2;

		//aplicamos una fuerza random..
		force.x = rand()/RAND_MAX;
		force.y = rand() / RAND_MAX;
		force.z = rand() / RAND_MAX;
		force = glm::normalize(force)*MaxForceApplied;
		//...en una posicion random del cubo
		forceApplyPos.x = ((rand() / RAND_MAX) - 0.5)*size / 2;
		forceApplyPos.y = ((rand() / RAND_MAX) - 0.5)*size / 2;
		forceApplyPos.z = ((rand() / RAND_MAX) - 0.5)*size / 2;

		//calculamos el torque
		torque = glm::cross(forceApplyPos, force);

		//momento linear
		linearMomentum = force;

		//momento angular
		angularMomentum = torque*0.033f;

		//orientacion
		rotation = glm::quat();
		force = gravity;
	
	}
	void Update(float dt) {
		//momento linear
		linearMomentum += force*dt;
		
		//momento angular
		//angularMomentum += torque*dt;

		//velocidad
		linearSpeed = linearMomentum / mass;

		//posicion
		position += linearSpeed*dt;

		//tensor de inercia
		InertiaTensor = glm::mat3_cast(rotation)*InertiaTensorBody*glm::transpose( glm::mat3_cast(rotation));
		
		//velocidad angular
		angularSpeed = InertiaTensor*angularMomentum;

		//orientacion
		rotation +=dt* (1.f/2)*(glm::quat(0,angularSpeed)*rotation);
	}
	
	//dimensiones
	float size;
	float mass;

	//control del tiempo
	//glm::vec3 randomPosMaxPos, randomPosMinPos;
	float time;
	float maxTime;
	//fuerzas
	float MaxForceApplied;
	glm::vec3 force;
	glm::vec3 forceApplyPos;
	glm::vec3 torque;
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


Cube cubo;
bool firstFrame = 0;

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
	if (cubo.time>cubo.maxTime) {
		cubo.reStart();
	}
	cubo.Update(dt);
}
void PhysicsCleanup() {
	//TODO
}