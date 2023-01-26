#include "ParticleSystem.h"

void ParticleSystem::Init(int particlePoolSize, RenderMode mode, Shader& particleShader)
{
	particlePool = std::vector<ParticleData>(particlePoolSize);
	particlePoolIndex = particlePoolSize - 1;

	shader = &particleShader;
	
	switch (mode)
	{
	case RenderMode::Render2D:
		renderFunctionBegin = &ParticleSystem::_Render2DBegin;
		renderFunctionDraw = &ParticleSystem::_Render2DDraw;
		renderFunctionEnd = &ParticleSystem::_Render2DEnd;
		renderFunctionDispose = &ParticleSystem::_Render2DDispose;
		break;
	case RenderMode::Facing:
		renderFunctionBegin = &ParticleSystem::_FacingBegin;
		renderFunctionDraw = &ParticleSystem::_FacingDraw;
		renderFunctionEnd = &ParticleSystem::_FacingEnd;
		renderFunctionDispose = &ParticleSystem::_FacingDispose;
		break;
	}
	renderMode = mode;
}

void ParticleSystem::Dispose()
{
	renderFunctionDispose(*shader);
}

void ParticleSystem::SetPhysics(ParticlePhysic physic)
{
	this->physic = physic;
}

void ParticleSystem::Emit(const ParticleProperties& properties)
{
	ParticleData& particle = particlePool[particlePoolIndex];
	particle.Active = true;
	particle.LifeTime = properties.LifeTime;
	particle.RemainingTime = properties.LifeTime;
	particle.TextureID = properties.TextureID;

	particle.Position.x = properties.Position.x + properties.PositionVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.Position.y = properties.Position.y + properties.PositionVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.Position.z = properties.Position.z + properties.PositionVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.Rotation.x = properties.Rotation.x + properties.RotationVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.Rotation.y = properties.Rotation.y + properties.RotationVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.Rotation.z = properties.Rotation.z + properties.RotationVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.Scale.x = properties.Scale.x + properties.ScaleVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.Scale.y = properties.Scale.y + properties.ScaleVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.Scale.z = properties.Scale.z + properties.ScaleVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.Color.x = properties.Color.x + properties.ColorVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.Color.y = properties.Color.y + properties.ColorVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.Color.z = properties.Color.z + properties.ColorVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.Color.w = properties.Color.w + properties.ColorVariation.w * (Random::Float() - 0.5f) * 2.0f;
	
	particle.Velocity.x = properties.Velocity.x + properties.VelocityVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.Velocity.y = properties.Velocity.y + properties.VelocityVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.Velocity.z = properties.Velocity.z + properties.VelocityVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.Torque.x = properties.Torque.x + properties.TorqueVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.Torque.y = properties.Torque.y + properties.TorqueVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.Torque.z = properties.Torque.z + properties.TorqueVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.ScaleChange.x = properties.ScaleChange.x + properties.ScaleChangeVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.ScaleChange.y = properties.ScaleChange.y + properties.ScaleChangeVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.ScaleChange.z = properties.ScaleChange.z + properties.ScaleChangeVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.ColorChange.x = properties.ColorChange.x + properties.ColorChangeVariation.x * (Random::Float() - 0.5f) * 2.0f;
	particle.ColorChange.y = properties.ColorChange.y + properties.ColorChangeVariation.y * (Random::Float() - 0.5f) * 2.0f;
	particle.ColorChange.z = properties.ColorChange.z + properties.ColorChangeVariation.z * (Random::Float() - 0.5f) * 2.0f;
	particle.ColorChange.w = properties.ColorChange.w + properties.ColorChangeVariation.w * (Random::Float() - 0.5f) * 2.0f;
	
	if (particlePoolIndex == 0)
		particlePoolIndex = particlePool.size();
	particlePoolIndex--;
}

void ParticleSystem::Update(TimeStep timestep)
{
	for (auto& particle : particlePool)
	{
		if (!particle.Active)
			continue;
		if (particle.RemainingTime <= 0.0f)
		{
			particle.Active = false;
			particle.RemainingTime = 0.0f;
			continue;
		}
		float deltaTime = timestep;
		particle.RemainingTime -= deltaTime;
		particle.Position += particle.Velocity * deltaTime;
		particle.Velocity += physic.Acceleration * deltaTime 
			- particle.Velocity * physic.LinearDrag * deltaTime;

		particle.Rotation += particle.Torque * deltaTime;
		particle.Torque += physic.AngularAcceleration * deltaTime
			- particle.Torque * physic.AngularDrag * deltaTime;
		
		particle.Scale += particle.ScaleChange * deltaTime;
		particle.ScaleChange += physic.ScaleChangeAcceleration* deltaTime
			- particle.ScaleChange * physic.ScaleDrag * deltaTime;

		particle.Color += particle.ColorChange * deltaTime;
		particle.ColorChange += physic.ColorChangeAcceleration * deltaTime;

		particle.Color = glm::clamp(particle.Color, glm::vec4(0.0f), glm::vec4(1.0f));
	}
}

void ParticleSystem::Render()
{
	shader->Begin();

	renderFunctionBegin(*shader);
	int count = 0;
	int maxCount = particlePool.size();
	for (int i = particlePoolIndex - 1; i >= 0; i--)
	{
		ParticleData& particle = particlePool[i];
		if (!particle.Active)
			continue;
		renderFunctionDraw(*shader, particle, count, maxCount);
		count++;
	}
	for (int i = particlePool.size() - 1; i >= particlePoolIndex; i--)
	{
		ParticleData& particle = particlePool[i];
		if (!particle.Active)
			continue;
		renderFunctionDraw(*shader, particle, count, maxCount);
		count++;
	}
	/*
	for (auto& particle : particlePool)
	{
		if (!particle.Active)
			continue;
		renderFunctionDraw(*shader, particle, count, maxCount);
		count++;
	}
	*/
	renderFunctionEnd(*shader);

	shader->End();
}

void ParticleSystem::_Render2DBegin(Shader& shader)
{
	Renderer2D::Begin(&shader, false);

	glDisable(GL_DEPTH_TEST);
}
void ParticleSystem::_Render2DDraw(Shader& shader, const ParticleData& data,
	const int& currentCount, const int& maxCount)
{
	glm::vec2 pos = glm::vec2(data.Position.x - data.Scale.x / 2.0f, data.Position.y - data.Scale.y / 2.0f);
	glm::vec2 size = glm::vec2(data.Scale.x, data.Scale.y);
	Renderer2D::DrawQuad(pos, size, 0.0f, 
		data.Color, data.TextureID, glm::vec2(data.Position.x, data.Position.y), data.Rotation.y);
}
void ParticleSystem::_Render2DEnd(Shader& shader)
{
	Renderer2D::End();
}
void ParticleSystem::_Render2DDispose(Shader& shader)
{
	Renderer2D::End();
}

void ParticleSystem::_FacingBegin(Shader& shader)
{
}
void ParticleSystem::_FacingDraw(Shader& shader, const ParticleData& data,
	const int& currentCount, const int& maxCount)
{
}
void ParticleSystem::_FacingEnd(Shader& shader)
{
}
void ParticleSystem::_FacingDispose(Shader& shader)
{
}
