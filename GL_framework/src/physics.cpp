#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <iostream>

bool show_test_window = false;
namespace Cubo {
	extern void cubeSetup();
	extern void cleanupClothMesh();
	extern void updateCube(const glm::mat4 &transform);
	extern void drawCube();
	extern float halfW;
}
glm::vec3 gravity(0,-9.8,0);

//planos de la caja


glm::vec3 planeNormal[6] = {
	glm::vec3(0,1,0),
	glm::vec3(0, -1, 0),
	glm::vec3(-1, 0, 0),
	glm::vec3(1, 0, 0),
	glm::vec3(0, 0, 1),
	glm::vec3(0, 0, -1)
};

float planeD[6] = {
	0,
	10,
	5,
	5,
	5,
	5
};

//glm::vec3 lowPlaneNormal
//glm::vec3 upperPlaneNormal(0, -1, 0);
//glm::vec3 rightPlaneNormal(-1, 0, 0);
//glm::vec3 leftPlaneNormal(1, 0, 0);
//glm::vec3 frontPlaneNormal(0, 0, 1);
//glm::vec3 backPlaneNormal(0, 0, -1);
//float lowPlaneD = 0;
//float upperPlaneD = 10;
//float rightPlaneD = 5;
//float leftPlaneD = 5;
//float frontPlaneD = 5;
//float backPlaneD = 5;

class Cube{
public:
	Cube() {
		//datos
		size = Cubo::halfW * 2;
		mass = 1;
		//tensor de incercia inicial (Ibody)
		InertiaTensorBody[0] = glm::vec3((1.f/12)*mass*(pow(size, 2)*2),0,0);
		InertiaTensorBody[1] = glm::vec3(0,(1.f / 12)*mass*(pow(size, 2) * 2),0);
		InertiaTensorBody[2] = glm::vec3(0,0,(1.f / 12)*mass*(pow(size, 2) + pow(size, 2)));

		InertiaTensorBody = glm::inverse(InertiaTensorBody);

		//maximo tamaño de la fuerza
		MaxForceApplied = 10;

		maxTime = 5;
		time = maxTime + 1;
		

		maxNumOfIterations = 10;
		accuracity = 0.001;

		//guardar los puntos originales

		originalVertex[0] = glm::vec3(-Cubo::halfW, -Cubo::halfW, Cubo::halfW);
		originalVertex[1] = glm::vec3(Cubo::halfW, -Cubo::halfW, Cubo::halfW);
		originalVertex[2] = glm::vec3(-Cubo::halfW, -Cubo::halfW, -Cubo::halfW);
		originalVertex[3] = glm::vec3(Cubo::halfW, -Cubo::halfW, -Cubo::halfW);
		originalVertex[4] = glm::vec3(-Cubo::halfW, Cubo::halfW, Cubo::halfW);
		originalVertex[5] = glm::vec3(Cubo::halfW, Cubo::halfW, Cubo::halfW);
		originalVertex[6] = glm::vec3(-Cubo::halfW, Cubo::halfW, -Cubo::halfW);
		originalVertex[7] = glm::vec3(Cubo::halfW, Cubo::halfW, -Cubo::halfW);
	}
	~Cube() {

	}
	void reStart() {
		//inicializar parametros
		time = 0;

		//inicializamos posicion
		position.x = ((((float)rand()) / RAND_MAX) - 0.5) *(5-size);
		position.y = (((float)rand()) / RAND_MAX)*5+size/2;
		position.z = ((((float)rand()) / RAND_MAX)*5)-size/2;

		//aplicamos una fuerza random..
		force.x = ((float)rand())/RAND_MAX;
		force.y = ((float)rand()) / RAND_MAX;
		force.z = ((float)rand()) / RAND_MAX;
		force = glm::normalize(force)*MaxForceApplied;

		//...en una posicion random del cubo
		forceApplyPos.x = (((((float)rand()) / RAND_MAX) - 0.5)*size / 4)+position.x;
		forceApplyPos.y = (((((float)rand()) / RAND_MAX) - 0.5)*size / 4)+position.y;
		forceApplyPos.z = (((((float)rand()) / RAND_MAX) - 0.5)*size / 4)+position.z;
		if (forceApplyPos.x >= 0)forceApplyPos.x += size / 2;
		else forceApplyPos.x -= size / 2;
		if (forceApplyPos.y >= 0)forceApplyPos.x += size / 2;
		else forceApplyPos.y -= size / 2;
		if (forceApplyPos.z >= 0)forceApplyPos.x += size / 2;
		else forceApplyPos.z -= size / 2;


		//calculamos el torque
		torque = glm::cross(forceApplyPos, force);

		//momento linear
		linearMomentum = force;

		//momento angular
		angularMomentum = torque*0.033f;

		//orientacion
		rotation = glm::quat();
		force = gravity;

		//posiciones relativas a los planos

		calculateTransformMat();
		for (int i = 0; i < 6;i++) {
			for (int j = 0; j < 8; j++) {
				if (distancePointPlane(vertex[j], planeNormal[i], planeD[i]) <= 0) {
					relativePositions[i][j] = true;
				}
				else {
					relativePositions[i][j] = false;
				}
			}
		}
	
	}
	void Update(float dt) {
		//Guardar datos del paso anterior

		lastLinearMomentum = linearMomentum;
		lastLinearSpeed = linearSpeed;
		lastPosition = position;
		lastInertiaTensor = InertiaTensor;
		lastAngularSpeed = angularSpeed;
		lastRotation = rotation;

		//tiempo
		time += dt;

		//momento linear
		linearMomentum += force*dt;
		

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

		
		//checkear las colisiones
		checkCollisions(dt);

		//calcular la matriz de transformacion
		calculateTransformMat();
	}

	void calculateTransformMat() {
		translation = glm::translate(glm::mat4(), position);

		rotMat = glm::mat4(mat3_cast(rotation));

		model = translation*rotMat;
		for (int i = 0; i < 8;i++) {
			vertex[i] = model * glm::vec4(originalVertex[i], 1.f);
		}
	}
	glm::mat4 getTransformMat() {	
		return model;
	}

	float distancePointPlane(glm::vec3& point, glm::vec3 &normal, float d) {
		float ret = (glm::dot(point, normal) + d) / glm::length(normal);
		return ret;
	}

	bool checkWallRelativeDist(glm::vec3 &point, glm::vec3 &normal, float d) {	
	if (distancePointPlane(point, normal, d)<=0) {
			return true;
		}
		else {
			return false;
		}
	}
	
	float findCollTime(float& dt, int& planeNumber, int& vertexNumber) {
		bool resultFound=false;
		int i = 1;
		colDt = dt/2;
		while (!resultFound && i<maxNumOfIterations) {
			//reinicializamos las variables
			tmpLinearMomentum = lastLinearMomentum;
			tmpLinearSpeed = lastLinearSpeed;
			tmpPosition = lastPosition;
			tmpAngularSpeed = lastAngularSpeed;
			tmpRotation = lastRotation;
			tmpInertiaTensor = lastInertiaTensor;
			
			//hacemos el paso de euler para el dt 
			tmpLinearMomentum += force*colDt;
			tmpLinearSpeed = linearMomentum / mass;
			tmpPosition += tmpLinearSpeed*colDt;
			tmpInertiaTensor = glm::mat3_cast(tmpRotation)*InertiaTensorBody*glm::transpose(glm::mat3_cast(tmpRotation));
			tmpAngularSpeed = tmpInertiaTensor*angularMomentum;
			tmpRotation += colDt* (1.f / 2)*(glm::quat(0, tmpAngularSpeed)*tmpRotation);

			//hacemos la matriz de transformacion para estos datos
			
			tmpModel = glm::translate(glm::mat4(), position) * glm::mat4(mat3_cast(rotation));

			//movemos los vertices
			
			tmpVertex = tmpModel*glm::vec4(originalVertex[planeNumber], 1.f);

			//comprobamos si el resultado nos sirve como aproximacion
			if (abs(distancePointPlane(tmpVertex, planeNormal[planeNumber], planeD[planeNumber])) <= accuracity) {
				resultFound = true;
			}
	
			//si no nos sirve hacemos el siguiente paso
			if (!resultFound ) {
				if (checkWallRelativeDist(tmpVertex, planeNormal[planeNumber], planeD[planeNumber]) != relativePositions[planeNumber][vertexNumber]) {
					colDt -= dt / (2 * pow(2, i));
				}
				else {
					colDt += dt / (2 * pow(2, i));
				}
				
			}
			i++;
		}

		return colDt;

	}

	void continueUpdate(float dt) {
		
	}

	void applyColl(float& dt, int& planeNumber, int& pointNumber) {
		float contDt = dt - findCollTime(dt, planeNumber, pointNumber);
		
		continueUpdate(contDt);
	}

	void checkWallColl(int& i, int& j, glm::vec3 &normal, float d, float& dt) {
		if (checkWallRelativeDist(vertex[i], normal, d)!=relativePositions[j][i]) {
			applyColl(dt, j, i);
		}
	
	}
	
	

	void checkCollisions(float& dt) {
		for (int i = 0; i < 8;i++) {
			for (int j = 0; j < 6;j++) {
				checkWallColl(i, j, planeNormal[j], planeD[j], dt);
			}
		}
		
	}
	
	//dimensiones
	float size;
	float mass;

	//control del tiempo
	//glm::vec3 randomPosMaxPos, randomPosMinPos;
	float time;
	float maxTime;
	
	//matriz de transformacion
	glm::mat4 model;
	glm::mat4 tmpModel;
	glm::mat4 rotMat;
	glm::mat4 translation;

	//fuerzas
	float MaxForceApplied;
	glm::vec3 force;
	glm::vec3 forceApplyPos;
	glm::vec3 torque;
	glm::vec3 linearMomentum, lastLinearMomentum, tmpLinearMomentum;
	glm::vec3 angularMomentum;

	//translacion
	glm::vec3 position, lastPosition, tmpPosition;
	glm::vec3 linearSpeed, lastLinearSpeed, tmpLinearSpeed;

	//rotación
	//su direccion es el eje de rotacion y su modulo la velocidad
	glm::vec3 angularSpeed, lastAngularSpeed, tmpAngularSpeed;
	glm::quat rotation, lastRotation, tmpRotation;
	glm::mat3 InertiaTensorBody;
	glm::mat3 InertiaTensor, lastInertiaTensor, tmpInertiaTensor;

	//colisiones
	float colDt;
	bool relativePositions[6][8];
	glm::vec3 originalVertex[8];
	glm::vec3 vertex[8];
	glm::vec3 tmpVertex;
	float accuracity;
	float maxNumOfIterations;

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

	Cubo::updateCube(cubo.getTransformMat());
}
void PhysicsCleanup() {
	//TODO
}