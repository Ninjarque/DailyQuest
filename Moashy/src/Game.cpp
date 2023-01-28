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

		if (!_cube.Has<Body>())
		{
			Physics2D::MakeBox(_cube);
		}
		_cube.Get<Body>().IsDynamic = _cubePhysique;

		//on cr�� une entity depuis la sc�ne encore
		_plateau = _story->CreateEntity();

		//on r�cup�re la taille de l'�cran
		Window::Current->GetSize(Width, Height);

		//on transforme le plateau pour qu'il ai de la physique, en donnant sa
		Physics2D::MakeBox(_plateau,
			//position
			glm::vec2(Width / 2.0f, Height - 20.0f),
			//taille
			glm::vec2(Width / 4.0f, 20.0f),
			//angle
			0.0f,
			//dire si il est statique
			true);

		//on lie le clique gauche au mot "Click"
		InputManager::SetBinding(InputType::Mouse, "Click", GLFW_MOUSE_BUTTON_1);
		InputManager::SetBinding(InputType::Mouse, "Physique", GLFW_MOUSE_BUTTON_2);
	}

	//Tout le code pour les initialisations du jeu ici


}

void Game::OnDispose()
{
}

void Game::OnUpdate(TimeStep timestep)
{
	//code pour r�cup�rer la taille de l'�cran
	Window::Current->GetSize(Width, Height);

	//on set la taille du plateau suivant la taille de l'�cran
	_plateau.Set<Size>(Width / 4.0f, 20.0f);

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
		//on d�sactive l'effet dynamique de l'entity
		_cube.Get<Body>().IsDynamic = false;
	}
	//on v�rifie si on vient tout juste de rel�cher la touche
	if (InputManager::IsReleased("Click"))
	{
		//on remet la dynamique comme elle �tait
		_cube.Get<Body>().IsDynamic = _cubePhysique;
	}
	//on v�rifie si on a fait l'action � cette frame uniquement
	if (InputManager::IsPressed("Physique"))
	{
		//on inverse la valeur � chaque frame
		_cubePhysique = !_cubePhysique;
		//on applique la nouvelle dynamique/le fait d'�tre statique
		_cube.Get<Body>().IsDynamic = _cubePhysique;
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
