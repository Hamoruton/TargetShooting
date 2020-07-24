#include "Game.h"

//=================================================================
// ƒƒCƒ“ƒ‹[ƒ`ƒ“
//=================================================================
int main(int args, char** argv)
{
	CGame* pGame = new CGame;

	bool success = pGame->Initialize();
	if (success) 
	{
		pGame->RunLoop();
	}

	delete pGame;
	return 0;
}