#include "Game.h"

void Game::OnInit()
{
	//Initialisation obligatoire
	Renderer2D::Init(128);
	shader.Init("res/Shaders/Basic.shader");

	camera = Camera(glm::vec2(0.0f, 0.0f), 1.0f);

	//Tout le code pour les initialisation du jeu ici
}

void Game::OnDispose()
{
}

void Game::OnUpdate(float deltaTime)
{
}

void Game::OnDraw()
{
}

void Game::OnImGUIDraw()
{
}
