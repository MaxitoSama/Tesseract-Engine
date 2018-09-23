#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleGUI.h"

using namespace std;

Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio(true);
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	gui = new ModuleGUI();

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);

	// Scenes
	AddModule(scene_intro);
	
	// Gui
	AddModule(gui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	////p2List_item<Module*>* item = list_modules.getLast();

	for (list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend() && (*item)==NULL; item++)
	{
		delete (*item);
	}
}

bool Application::Init()
{
	bool ret = true;

	//TO be able to use RNG in any file without worrying about initializing the seed
	START_RNG_SEED();

	// Call Init() in all modules
	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == true; item++)
	{
		ret = (*item)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret==true; item++)
	{
		ret = (*item)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.ReadTime() / 1000.0f;

	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	int ms_cap = 1000 / framerateCap;
	if (ms_timer.ReadTime() < ms_cap)
		SDL_Delay(ms_cap - ms_timer.ReadTime());

	gui->logFPS(1 / dt, dt * 1000);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->PreUpdate(dt);
	}

	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->Update(dt);
	}

	for (list<Module*>::iterator item = list_modules.begin(); item != list_modules.end() && ret == UPDATE_CONTINUE; item++)
	{
		ret = (*item)->PostUpdate(dt);
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	list<Module*>::iterator item = list_modules.begin();

	for (list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend() && ret == true; item++)
	{
		ret = (*item)->CleanUp();
	}

	return ret;
}

const char* Application::getAppName() const
{
	return appName.c_str();
}

void Application::setAppName(const char * name)
{
	if (name != nullptr && name != appName)
	{
		appName = name;
		SDL_SetWindowTitle(window->window, name);
	}
}

int Application::getFramerateCap() const
{
	return framerateCap;
}

void Application::setFramerateCap(int cap)
{
	framerateCap = cap;
}

void Application::RequestBrowser(const char * url) const
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}