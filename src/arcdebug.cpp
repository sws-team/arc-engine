#include "arcdebug.h"
#include "engine.h"
#include "mainwindow.h"
#include "arcobject.h"
#include "managers.h"

#ifdef ARC_DEBUG
#include "imgui.h"
#include "imgui-SFML.h"
#endif

ArcDebug::ArcDebug()
{

}

void ArcDebug::setObject(ArcObject *object)
{
	this->object = object;
}

void ArcDebug::draw(sf::RenderTarget *target)
{
	drawFrame();
#ifdef ARC_DEBUG
	ImGui::SFML::Render(*target);
#endif
}

void ArcDebug::init()
{
#ifdef ARC_DEBUG
	ImGui::SFML::Init(*static_cast<sf::RenderWindow*>(Engine::Instance().window()));
#endif
}

void ArcDebug::clear()
{
#ifdef ARC_DEBUG
	ImGui::SFML::Shutdown();
#endif
}

void ArcDebug::update()
{
#ifdef ARC_DEBUG
	ImGui::SFML::Update(*static_cast<sf::RenderWindow*>(Engine::Instance().window()), clock.restart());
#endif
	const float currentTime = FPS.clock.restart().asSeconds();
	if (FPS.updateTimer.check(ArcEngine::MSEC)) {
		FPS.value = 1.f / currentTime;
	}
}

void ArcDebug::eventFilter(sf::Event *event)
{
#ifdef ARC_DEBUG
	ImGui::SFML::ProcessEvent(*event);
#endif
}

void ArcDebug::drawFrame()
{
#ifdef ARC_DEBUG
//	ImGuiIO &io = ImGui::GetIO();
	static bool showDebug = true;
	const ImVec2 windowSize = ImVec2(Engine::Instance().settingsManager()->getResolutionF().x / 3,
									 Engine::Instance().settingsManager()->getResolutionF().y / 3);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.5f);
	if (ImGui::Begin("Debug", &showDebug, ImGuiWindowFlags_None)) {
		if (ImGui::BeginTabBar("DebugTabs")) {
			if (ImGui::BeginTabItem("Common")) {

				ImGui::TextUnformatted("Arc Engine ver.#");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.8f, 0.92f, 0.45f, 1.f), ENGINE_VERSION);
				ImGui::TextUnformatted("FPS:");
				ImGui::SameLine();
				const float fpsColorV = static_cast<float>(FPS.value) * ArcEngine::FRAME_TIME;
				ImVec4 fpsColor;
				fpsColor.w = 1.f;
				fpsColor.x = 1.f - fpsColorV;
				fpsColor.y = fpsColorV;
				fpsColor.z = 0;
				ImGui::TextColored(fpsColor, "%s", std::to_string(FPS.value).c_str());

				ImGui::EndTabItem();
			}
			static bool firstOpen = true;
			const ImGuiTabItemFlags_ itemFlags = firstOpen ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None;
			if (firstOpen)
				firstOpen = false;
			if (ImGui::BeginTabItem("Objects", nullptr, itemFlags)) {
				drawObject(object);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Other")) {
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	ImGui::ShowDemoWindow();
#endif
}

void ArcDebug::drawObject(ArcObject *obj)
{
	if (obj == nullptr)
		return;
#ifdef ARC_DEBUG
	const float height = ImGui::GetWindowSize().y;
	static float w = 200.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
	ImGui::BeginChild("Objects tree", ImVec2(w, 0), true);
	ImGui::TextUnformatted("Objects tree");
	std::function<void(ArcObject *)> drawCurrentObject;
	drawCurrentObject = [&drawCurrentObject](ArcObject *object)-> void {
		if (ImGui::TreeNode(object->name().c_str())) {
			for (unsigned i = 0; i < object->childs.size(); ++i) {
				ArcObject* child = object->childs.at(i);
				ImGui::Indent();
				drawCurrentObject(child);
				ImGui::Unindent();
			}
			ImGui::TreePop();
		}
	};
	drawCurrentObject(obj);
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::InvisibleButton("vsplitter", ImVec2(8.0f, height));
	if (ImGui::IsItemActive())
		w += ImGui::GetIO().MouseDelta.x;

	ImGui::SameLine();
	ImGui::BeginChild("Properties", ImVec2(0, 0), true);
	ImGui::TextUnformatted("Properties");
	//FIXME selected obj
	drawObjectProperties(obj);
	ImGui::EndChild();
	ImGui::PopStyleVar();
#endif
}

void ArcDebug::drawObjectProperties(ArcObject *obj)
{
#ifdef ARC_DEBUG
	auto drawInheritObject = [](ArcEngine::OBJECT_TYPE type, ArcObject *obj) {
		switch (type)
		{
		case ArcEngine::OBJECT:
		{
			// x y angle scale origin size
		}
			break;
		case ArcEngine::SPRITE:
		{

		}
			break;
		case ArcEngine::LABEL:
		{

		}
			break;
		case ArcEngine::BUTTON:
		{

		}
			break;
		default:
			break;
		}
	};

	drawInheritObject(ArcEngine::OBJECT, obj);
	switch (obj->type())
	{
	case ArcEngine::SPRITE:
		drawInheritObject(ArcEngine::SPRITE, obj);
		break;
	case ArcEngine::LABEL:
		drawInheritObject(ArcEngine::LABEL, obj);
		break;
	case ArcEngine::BUTTON:
		drawInheritObject(ArcEngine::SPRITE, obj);
		drawInheritObject(ArcEngine::BUTTON, obj);
		break;
	default:
		break;
	}
#endif
}

std::string ArcDebug::typeToName(ArcEngine::OBJECT_TYPE type)
{
	switch (type)
	{
	case ArcEngine::UNDEF:
		return "Undef";
		break;
	case ArcEngine::OBJECT:
		return "Object";
		break;
	case ArcEngine::SPRITE:
		return "Sprite";
		break;
	case ArcEngine::LABEL:
		return "Label";
		break;
	case ArcEngine::BUTTON:
		return "Button";
		break;
	default:
		break;
	}
	return std::string();
}
