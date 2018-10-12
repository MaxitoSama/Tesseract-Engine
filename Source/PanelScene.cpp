#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleGUI.h"
#include "PanelScene.h"
#include "ModuleInput.h"


PanelScene::PanelScene(const char* name, float posX, float posY, float width, float height, panelAlingnment aligned) : Panel(name, posX, posY, width, height, aligned)
{
	active = true;
}


PanelScene::~PanelScene()
{
}

void PanelScene::Draw()
{

	ImGui::SetNextWindowSizeConstraints({ 10,10 }, { (float)App->window->width, (float)App->window->height });
	ImGui::Begin(name.c_str(), &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	size = ImGui::GetWindowSize();
	ImGui::SetCursorPos({ -(App->window->width - size.x) / 2,-(App->window->height - size.y) / 2 });

	ImGui::Image((ImTextureID)App->renderer3D->renderedTexture, { (float)App->window->width, (float)App->window->height }, { 0,1 }, { 1,0 });
	
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) != KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) != KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_MIDDLE) != KEY_REPEAT)
		App->gui->hoveringScene = ImGui::IsMouseHoveringWindow();

	ImGui::End();

}