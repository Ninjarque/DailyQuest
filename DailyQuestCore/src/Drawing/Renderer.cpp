#include "Renderer.h"

std::unordered_map<Shader::ShaderType, Shader*> Renderer::_shaders;
Camera* Renderer::_camera = nullptr;
Viewport* Renderer::_viewport = nullptr;

Shader* Renderer::_defaultShader = nullptr;

void Renderer::Init(const std::string shaderFile)
{
	_defaultShader = new Shader(shaderFile, Shader::ShaderType::Default);
	Renderer2D::Init(128);
}

void Renderer::Draw(Story& story)
{
	_shaders.clear();
	_shaders[Shader::ShaderType::Default] = _defaultShader;
	_camera = nullptr;
	_viewport = nullptr;

	story.ComputeForEachEntity<Shader>(ExtractShader);
	story.ComputeForEachEntity<Camera>(ExtractCamera);
	bool noCam = false;
	if (_camera == nullptr)
	{
		_camera = new Camera(story.CreateEntity());
		noCam = true;
	}
	/*
	bool noViewport = false;
	if (_viewport == nullptr)
	{
		float x = 0.0f, y = 0.0f;
		int w, h;
		Window::Current->GetSize(w, h);
		_viewport = new Viewport(x, y, (float)w, (float)h);
		noViewport = true;
	}
	*/

	Shader* shader = _shaders[Shader::ShaderType::Default];
	shader->Begin();
	Renderer2D::Begin(_camera, shader, _viewport);
	story.ComputeForEachEntity<Location, Size>(Render2D);
	Renderer2D::End();
	shader->End();

	//if (noCam)
		delete _camera;
	if (_viewport != nullptr)//noViewport)
		delete _viewport;
}

void Renderer::ExtractShader(Entity entity, Shader& shader)
{
	_shaders[shader.GetShaderType()] = &shader;
}

void Renderer::ExtractCamera(Entity entity, Camera& camera)
{
	if (camera.IsActive())
	{
		if (_camera != nullptr)
			delete _camera;
		_camera = new Camera(camera);
		if (entity.Has<Viewport>())
		{
			if (_viewport != nullptr)
				delete _viewport;
			_viewport = new Viewport(entity.Get<Viewport>());
		}
	}
}

void Renderer::Render2D(Entity entity, Location& location, Size& size)
{
	//if (entity.Has<ParticleSystem>()) return;
	//if (entity.Has<Font>()) return;
	
	Color color;
	Texture* texture = nullptr;
	Depth depth;

	if (entity.Has<Color>())
		color = entity.Get<Color>();
	if (entity.Has<Texture>())
	{
		texture = new Texture(entity.Get<Texture>());
	}
	if (entity.Has<Depth>())
		depth = entity.Get<Depth>();
	Origin origin(location.X, location.Y);
	if (entity.Has<Origin>())
		origin = entity.Get<Origin>();
	Angle angle;
	if (entity.Has<Angle>())
		angle = entity.Get<Angle>();
	UVRect uvRect;
	if (entity.Has<UVRect>())
		uvRect = entity.Get<UVRect>();


	Renderer2D::DrawQuad(
		glm::vec2(location.X - size.X / 2.0f, location.Y - size.Y / 2.0f),
		glm::vec2(size.X, size.Y),
		depth.Value,
		glm::vec4(color.R, color.G, color.B, color.A),
		texture,
		glm::vec2(uvRect.X, uvRect.Y),
		glm::vec2(uvRect.Width, uvRect.Height),
		glm::vec2(origin.X, origin.Y),
		angle.Value);


	if (texture != nullptr) delete texture;
}
