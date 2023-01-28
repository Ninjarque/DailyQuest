#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Core/Window.h"
#include "Story/Entity.h"

#include "Basic.h"


namespace Components
{
	struct Zoom
	{
		float Value;
		Zoom() : Value(1.0f) { }
		Zoom(float value) : Value(value) { }
	};
	class Camera
	{
	public:
		Camera(Entity entity)
		{
			_entity = entity;
			this->position = glm::vec2(0.0f, 0.0f);
			this->angle = 0.0f;
			this->zoom = 1.0f;
			matrix = glm::mat4(1.0f);
			needsRecalculation = true;
			_isActive = true;
		}
		Camera(Entity entity, glm::vec2 position, float zoom)
		{
			_entity = entity;
			this->position = position;
			this->angle = 0.0f;
			this->zoom = zoom;
			matrix = glm::mat4(1.0f);
			needsRecalculation = true;
			_isActive = true;
		}
		~Camera() {

		}

		void Move(glm::vec2 amount)
		{
			position += amount;
			needsRecalculation = true;
		}
		void Rotate(float amount)
		{
			angle += amount;
			needsRecalculation = true;
		}
		void ZoomIn(float amount)
		{
			zoom += zoom * amount;
			needsRecalculation = true;
		}
		bool IsActive() { return _isActive; }

		glm::mat4 GetTransforms()
		{
			GetTransformsFromEntity();
			if (needsRecalculation || Window::Current->NeedsViewportRecalculations())
			{
				matrix = CalculateTransforms();
				needsRecalculation = false;
			}
			return matrix;
		}

	protected:
		enum Type
		{
			Is2D,
			Is3D
		};

		Entity _entity;
		glm::vec2 position;
		float angle;
		float zoom;

		bool _isActive;

		glm::mat4 matrix;
		bool needsRecalculation;

		glm::vec2 GetLocationFromEntity()
		{
			glm::vec2 pos = glm::vec2(0.0f, 0.0f);
			if (_entity.Has<Location>())
			{
				Location l = _entity.Get<Location>();
				pos.x = l.X;
				pos.y = l.Y;
			}
			return pos;
		}
		float GetAngleFromEntity()
		{
			float a = 0.0f;
			if (_entity.Has<Angle>())
			{
				Angle angle = _entity.Get<Angle>();
				a = angle.Value;
			}
			return a;
		}
		float GetZoomFromEntity()
		{
			float z = 1.0f;
			if (_entity.Has<Zoom>())
			{
				Zoom zoom = _entity.Get<Zoom>();
				z = zoom.Value;
			}
			return z;
		}


		void GetTransformsFromEntity()
		{
			glm::vec2 newPos = GetLocationFromEntity();
			float newAngle = GetAngleFromEntity();
			float newZoom = GetZoomFromEntity();
			if (newPos.x != position.x || newPos.y != position.y || newAngle != angle || newZoom != zoom)
				needsRecalculation = true;
			position = newPos;
			angle = newAngle;
			zoom = newZoom;
		}

		glm::mat4 CalculateTransforms()
		{
			int width, height;
			Window::Current->GetSize(width, height);
			glm::mat4 ortho = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1000.0f);
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f));
			glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(zoom));
			glm::mat4 res = ortho * translationMatrix * scaleMatrix;

			return res;//glm::inverse(res);
		}
	};
}