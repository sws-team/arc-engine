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
	drawObject(object);
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
}

void ArcDebug::eventFilter(sf::Event *event)
{
#ifdef ARC_DEBUG
	ImGui::SFML::ProcessEvent(*event);
#endif
}

void ArcDebug::drawObject(ArcObject *obj)
{
	if (obj == nullptr)
		return;
#ifdef ARC_DEBUG
	static bool showDebug = true;
	const ImVec2 windowSize = ImVec2(Engine::Instance().settingsManager()->getResolutionF().x / 3,
									 Engine::Instance().settingsManager()->getResolutionF().y / 3);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
//	ImGui::SetNextWindowBgAlpha(0.5f);
	if (ImGui::Begin("Debug", &showDebug, ImGuiWindowFlags_None)) {
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
		ImGui::EndChild();
		ImGui::PopStyleVar();

		ImGui::End();
	}
//	ImGui::ShowDemoWindow();
#endif
}
