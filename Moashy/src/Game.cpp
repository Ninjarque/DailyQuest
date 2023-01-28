#include "Game.h"

void Game::OnInit()
{
	//Initialisation obligatoire
	Renderer2D::Init(128);
	shader = new Shader("res/Shaders/Basic.shader", Shader::ShaderType::Default);

	//Tout le code pour les initialisation du jeu ici
}

void Game::OnDispose()
{
}

void Game::OnUpdate(TimeStep timestep)
{
}

void Game::OnDraw()
{
}

void Game::OnImGUIDraw()
{
}
