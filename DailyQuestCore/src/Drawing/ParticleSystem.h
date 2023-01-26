#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "Core/Random.h"
#include "Renderer2D.h"
#include "Shader.h"

struct ParticlePhysic
{
	glm::vec3 Acceleration = glm::vec3(0.0f, -9.81f, 0.0f);
	glm::vec3 AngularAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ScaleChangeAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec4 ColorChangeAcceleration = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	float LinearDrag = 0.01f;
	float AngularDrag = 0.01f;
	float ScaleDrag = 0.01f;

	ParticlePhysic() {}
	ParticlePhysic(glm::vec3 acceleration, glm::vec3 angularAcceleration, 
		glm::vec3 scaleChangeAcceleration, glm::vec4 colorChangeAcceleration,
		float linearDrag, float angularDrag, float scaleDrag) 
		: Acceleration(acceleration), AngularAcceleration(angularAcceleration),
		ScaleChangeAcceleration(scaleChangeAcceleration), ColorChangeAcceleration(colorChangeAcceleration),
		LinearDrag(linearDrag), AngularDrag(angularDrag), ScaleDrag(scaleDrag)
	{}
};
struct ParticleProperties
{
	float LifeTime = 1.0f;
	GLuint TextureID = 0;

	glm::vec3 Position = glm::vec3(0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f);
	glm::vec3 Scale = glm::vec3(1.0f);
	glm::vec4 Color = glm::vec4(1.0f);

	glm::vec3 PositionVariation = glm::vec3(0.0f);
	glm::vec3 RotationVariation = glm::vec3(0.0f);
	glm::vec3 ScaleVariation = glm::vec3(0.0f);
	glm::vec4 ColorVariation = glm::vec4(0.0f);

	glm::vec3 Velocity = glm::vec3(0.0f);
	glm::vec3 Torque = glm::vec3(0.0f);
	glm::vec3 ScaleChange = glm::vec3(0.0f);
	glm::vec4 ColorChange = glm::vec4(0.0f);

	glm::vec3 VelocityVariation = glm::vec3(0.0f);
	glm::vec3 TorqueVariation = glm::vec3(0.0f);
	glm::vec3 ScaleChangeVariation = glm::vec3(0.0f);
	glm::vec4 ColorChangeVariation = glm::vec4(0.0f);

	ParticleProperties() {}
	ParticleProperties(float lifeTime, glm::vec3 pos, glm::vec3 rot, 
		glm::vec3 scale, glm::vec4 color, GLuint textureID)
		: LifeTime(lifeTime), Position(pos), Rotation(rot), Scale(scale), Color(color),
		TextureID(textureID)
	{}
};

class ParticleSystem
{
public:
	enum RenderMode {
		Render2D,
		Billboard,
		Facing,
	};

	ParticleSystem() 
	{ 
		particlePoolIndex = 0; 
		renderMode = RenderMode::Facing; 
		renderFunctionBegin = &_FacingBegin;
		renderFunctionDraw = &_FacingDraw;
		renderFunctionEnd = &_FacingEnd;
		renderFunctionDispose = &_FacingDispose;

		shader = nullptr;
	}
	~ParticleSystem() { Dispose(); }

	void Init(int particlePoolSize, RenderMode mode, Shader& particleShader);
	void Dispose();

	void SetPhysics(ParticlePhysic physic);
	void Emit(const ParticleProperties& properties);

	void Update(TimeStep timestep);
	void Render();

private:
	struct ParticleData
	{
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Torque = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 ScaleChange = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 ColorChange = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

		float LifeTime = 1.0f;
		float RemainingTime = 1.0f;
		GLuint TextureID = 0;

		bool Active = false;
	};
	ParticlePhysic physic;

	std::vector<ParticleData> particlePool;
	int particlePoolIndex;

	Shader* shader;
	RenderMode renderMode;

	void(*renderFunctionBegin)(Shader& shader);
	void(*renderFunctionDraw)(Shader& shader, const ParticleData& data, 
		const int& currentCount, const int& maxCount);
	void(*renderFunctionEnd)(Shader& shader);
	void(*renderFunctionDispose)(Shader& shader);

	static void _Render2DBegin(Shader& shader);
	static void _Render2DDraw(Shader& shader, const ParticleData& data,
		const int& currentCount, const int& maxCount);
	static void _Render2DEnd(Shader& shader);
	static void _Render2DDispose(Shader& shader);
	
	static void _FacingBegin(Shader& shader);
	static void _FacingDraw(Shader& shader, const ParticleData& data,
		const int& currentCount, const int& maxCount);
	static void _FacingEnd(Shader& shader);
	static void _FacingDispose(Shader& shader);
};

