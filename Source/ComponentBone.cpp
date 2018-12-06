#include "ComponentBone.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleResource.h"
#include "ResourceBone.h"

#include "Component.h"
#include "ComponentTransformation.h"

#include "Primitive.h"

ComponentBone::ComponentBone(GameObject* parent, componentType type) : Component(parent, type)
{
	sphere = new MSphere();
	sphere->color = { 1.0,0.0,0.0,1.0 };
	line = new MLine();
	line->color = { 1.0,0.0,0.0,1.0 };
}

ComponentBone::~ComponentBone()
{
	RELEASE(sphere);
	RELEASE(line);
}

bool ComponentBone::Update(float dt)
{
	if (debugDraw)
	{
		ComponentTransformation* transform = (ComponentTransformation*)gameObject->GetComponent(TRANSFORMATION);
		vec position = float3::zero;
		vec scale = float3::one;
		Quat rot = Quat::identity;
		transform->globalMatrix.Decompose(position, rot, scale);
		sphere->SetPos(position.x, position.y, position.z);

		if (gameObject->parent != nullptr && gameObject->parent->GetComponent(ANIMATION) == nullptr)
		{
			line->origin = position;
			float4x4 parentMat = ((ComponentTransformation*)gameObject->parent->GetComponent(TRANSFORMATION))->globalMatrix;
			parentMat.Decompose(position, rot, scale);
			line->destination = position;
		}

		sphere->Render();
		line->Render();
	}
	
	return true;
}

void ComponentBone::DrawInfo()
{
	ResourceBone* bone = (ResourceBone*)App->resources->GetResource(RUID);

	std::string tag = (bone != nullptr) ? bone->GetName() : "Unknown";

	if (ImGui::CollapsingHeader(("Bone - "+tag).c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick))
	{
		ImGui::Text("This component defines current game object as a bone for executing animations");

		ImGui::Checkbox("DebugDraw", &debugDraw);
		ImGui::Text("WIP - Show afecting meshes");
	}
}

void ComponentBone::Save(JSON_Value * component) const
{
	JSON_Value* bone = component->createValue();

	bone->addInt("Type", type);
	bone->addUint("UID", UID);
	bone->addBool("debugDraw", debugDraw);

	component->addValue("", bone);
}

void ComponentBone::Load(JSON_Value * component)
{
	debugDraw = component->getBool("debugDraw");
}