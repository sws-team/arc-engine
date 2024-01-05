#include "tutorial.h"
#include <ArcEngine>
#include "gameoptions.h"
#include "managers.h"
#include <ArcLabel>
#include <ArcAction>
#include <ArcEngineUtils>

#ifdef ARC_DEBUG
#include <imgui.h>
#endif

Tutorial::Tutorial()
	: ArcObject("Tutorial")
{
	setSize(SettingsManager::defaultResolution);
	Engine::Instance().getOptions()->addNotificationCallback(std::bind(&Tutorial::checkTriggers, this,
																	   std::placeholders::_1, std::placeholders::_2));
#ifdef ARC_DEBUG
	debug = new TutorialDebug();
	debug->p = this;
	Engine::Instance().debugManager()->addSection(debug);
#endif
}

Tutorial::~Tutorial()
{
#ifdef ARC_DEBUG
	Engine::Instance().debugManager()->removeSection(debug);
	delete debug;
#endif
}

bool Tutorial::isActive() const
{
	return currentTutorial != nullptr;
}

void Tutorial::block(const std::vector<std::string> &blocking)
{
	this->blocking = blocking;
}

void Tutorial::block(const std::string &blocking)
{
	this->blocking = { blocking };
}

void Tutorial::created(const std::string& name, ArcObject *object)
{
	gc.insert(std::make_pair(name, object));
}

void Tutorial::garbageCollect(const std::string& name)
{
	if (auto it = gc.find(name); it != gc.end()) {
		it->second->destroy();
		gc.erase(it);
	}
}

void Tutorial::garbageCollect()
{
	for(auto it = gc.begin(); it != gc.end(); it++) {
		it->second->destroy();
	}
	gc.clear();
}

void Tutorial::unblock()
{
	blocking.clear();
}

bool Tutorial::eventFilter(sf::Event *event)
{
	if (blocking.empty())
		return ArcObject::eventFilter(event);
	for(const std::string& name : blocking) {
		if (ArcObject *obj = ArcEngine::findChildPath(name); obj != nullptr) {
			obj->event(event);
		}
	}
	return true;
}

void Tutorial::nextStep()
{
	const auto current = std::move(currentTutorial->steps.front());
	currentTutorial->steps.pop_front();

	current.func();

	if (currentTutorial->steps.empty()) {
		//completed
		completeCurrent();
	}
}

void Tutorial::fill(const std::vector<TutorialEntry>& tutorials)
{
	this->tutorials = tutorials;
}

rapidjson::Value Tutorial::save(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
{
	completed.clear();
	rapidjson::Value data(rapidjson::kArrayType);
	for(const std::string& name : completed)
		data.PushBack(rapidjson::Value(rapidjson::StringRef(name.c_str())).Move(), allocator);
	return data;
}

void Tutorial::load(const rapidjson::Value &data)
{
	for(const auto& name : data.GetArray()) {
		const std::string str = name.GetString();
		completed.push_back(str);
	}
	if (completed.empty())
		return;
	tutorials.erase(std::remove_if(tutorials.begin(), tutorials.end(),
								   [this](const TutorialEntry& entry) {
		return std::find(completed.begin(), completed.end(), entry.name) != completed.end();
	}));
}

void Tutorial::checkTriggers(const std::string &name, const ArcVariant &value)
{
	if (currentTutorial != nullptr) {
		checkConditions(name, value);
		return;
	}
	for(const TutorialEntry& entry : tutorials) {
		if (entry.steps.empty())
			continue;
		const Condition condition = entry.steps.front().condition;
		if (condition.name == name && condition.value == value) {
			currentTutorial = new TutorialEntry(entry);
			nextStep();
		}
	}
}

void Tutorial::checkConditions(const std::string &name, const ArcVariant &value)
{
	const auto& step = currentTutorial->steps.front();
	if (step.condition.name != name)
		return;
	if (step.condition.value == value) {
		nextStep();
	}
}

void Tutorial::completeCurrent()
{
	completed.push_back(currentTutorial->name);
	delete currentTutorial;
	currentTutorial = nullptr;
	OPTIONS->save();
}
#ifdef ARC_DEBUG
TutorialDebug::TutorialDebug()
	: DebugSection("Tutorial")
{

}

void TutorialDebug::draw()
{
	if (p->currentTutorial != nullptr) {
		if (ImGui::Button("Complete")) {
			p->completeCurrent();
		}
		ImGui::SameLine();
		if (ImGui::Button("Next step")) {
			p->nextStep();
		}
	}
	for(const Tutorial::TutorialEntry& entry : p->tutorials) {
		bool isCurrent = false;
		if (p->currentTutorial != nullptr) {
			isCurrent = p->currentTutorial->name == entry.name;
		}
		if (isCurrent) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 0, 0)));
			ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);
		}
		const bool opened = ImGui::TreeNode(entry.name.c_str());
		if (isCurrent) {
			ImGui::PopStyleColor(1);
		}
		if (opened) {
			for(const Tutorial::TutorialStep& step : entry.steps) {
				const std::string text = step.name + " (" + step.condition.name + ")";
				if (p->currentTutorial != nullptr && p->currentTutorial->steps.front().name == step.name)
					ImGui::TextColored(ImVec4(ImColor(255, 0, 0)), "%s", text.c_str());
				else
					ImGui::TextUnformatted(text.c_str());
			}
			ImGui::TreePop();
		}
	}
}
#endif
