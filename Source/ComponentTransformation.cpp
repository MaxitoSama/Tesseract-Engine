#include "ComponentTransformation.h"
#include "Application.h"

ComponentTransformation::~ComponentTransformation()
{
}

void ComponentTransformation::DrawInfo()
{
	if (ImGui::CollapsingHeader("Transformation", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		ImGui::Text("Position:\n X: %f | Y: %f | Z: %f", position.x, position.y, position.z);
		float3 rot = rotation.ToEulerXYZ();
		rot *= 180 / pi;
		ImGui::Text("Rotation:\n X: %f | Y: %f | Z: %f", rot.x, rot.y, rot.z);
		ImGui::Text("Scale:\n X: %f | Y: %f | Z: %f", scale.x, scale.y, scale.z);
	}
}
bool ComponentTransformation::Update()
{
	return true;
}
