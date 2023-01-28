#include "Game.h"

void Game::OnInit()
{
	//Initialisation obligatoire
	Renderer::Init("res/Shaders/Basic.shader");
	_fontShader = new Shader("res/Shaders/TextShader.shader", Shader::ShaderType::Font);
	//Les story sont des sc�nes en gros, elles permettent de stocker les entity
	_story = StoryManager::CreateStory();

	//Initialisation pour te montrer
	{
		//on charge la texture de soldier
		_texture = Image::Load("res/Images/soldier.png");

		//on cr�� une entity depuis la sc�ne nomm�e story
		_cube = _story->CreateEntity();

		//on lui rajoute des propri�t�s type location, size, texture
		_cube.Add<Location>(200, 200);
		_cube.Add<Size>(100, 100);
		_cube.Add<Texture>(*_texture);

		//on lie le clique gauche au mot "Click"
		InputManager::SetBinding(InputType::Mouse, "Click", GLFW_MOUSE_BUTTON_1);
	}

	//Tout le code pour les initialisation du jeu ici


}

void Game::OnDispose()
{
}

void Game::OnUpdate(TimeStep timestep)
{
	//code pour montrer comment r�cup�rer la position de la souris
	float x, y;
	Mouse::GetPosition(x, y);

	//code pour tester une touche qui a �t� li�e
	if (InputManager::IsDown("Click"))
	{
		//code pour r�cup�rer la position d'une entity (ici cube)
		Location location = _cube.Get<Location>();

		//calculs pour faire un lissage de position, rien de particulier
		location.X += (x - location.X) * timestep * 10.0f;
		location.Y += (y - location.Y) * timestep * 10.0f;
		
		//code pour set la position d'une entity (ici cube)
		_cube.Set<Location>(location);
		/*
		//on peu aussi set la position d'une entity directement avec un get :
		_cube.Get<Location>().X += (x - location.X) * timestep * 10.0f;
		_cube.Get<Location>().Y += (y - location.Y) * timestep * 10.0f;
		*/
	}
}

void Game::OnDraw()
{
	//Dessine la sc�ne
	Renderer::Draw(*_story.get());
}

void Game::OnImGUIDraw()
{
}
