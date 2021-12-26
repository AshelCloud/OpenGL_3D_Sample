#include <iostream>

#include "Core/System.hpp"

int main()
{
	System* system = new System();
	
	bool Result = system->Initialize();
	if (Result)
	{
		while (!system->ShouldCloseWindow())
		{
			system->ProcessInput();
			system->Render();
		}
	}

	system->Release();
	delete system;
	system = nullptr;
}