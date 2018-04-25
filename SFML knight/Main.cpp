#include "Game.hpp"

int main()
{
		Game game;
		while (!game.GetWindow()->IsDone()) 
		{
			game.Update();
			game.Render();
			game.LateUpdate();
		}
}