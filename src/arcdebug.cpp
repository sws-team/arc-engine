#include "arcdebug.h"
#include "engine.h"
#include "managers.h"
#include "mainwindow.h"
#include "utils.h"
#include "arcobject.h"
#include "arcsprite.h"
#include "arcbutton.h"
#include "arclabel.h"
#include "arcanimatedsprite.h"
#include "arcskeletonanimation.h"
#include "arclayout.h"

#ifdef ARC_DEBUG
#include "imgui.h"
#include "imgui-SFML.h"
#include "imconfig-SFML.h"
#endif

ArcObject *ArcDebug::selectedObject = nullptr;

ArcDebug::ArcDebug()
{

}

void ArcDebug::setObject(ArcObject *object)
{
	selectedObject = nullptr;
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
	if (FPS.updateTimer.check(ArcEngine::MSEC/5)) {
		FPS.value = 1.f / currentTime;
	}
}

void ArcDebug::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::KeyPressed) {
		if (event->key.code == sf::Keyboard::F3) {
			visible = !visible;
		}
	}
#ifdef ARC_DEBUG
	ImGui::SFML::ProcessEvent(*event);
#endif
}

void ArcDebug::drawFrame()
{
#ifdef ARC_DEBUG
//	ImGuiIO &io = ImGui::GetIO();
	const ImVec2 windowSize = ImVec2(Engine::Instance().settingsManager()->getResolutionF().x / 3,
									 Engine::Instance().settingsManager()->getResolutionF().y / 3);
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.5f);
	if (ImGui::Begin("Debug", &visible, ImGuiWindowFlags_None)) {
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
//	ImGui::ShowDemoWindow();
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
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool isSelected = selectedObject == object;
		if (isSelected)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		bool opened = ImGui::TreeNodeEx(object->name().c_str(), node_flags);
		if (ImGui::IsItemClicked()) {
			selectedObject = object;
		}
		if (opened) {
			for (unsigned i = 0; i < object->childs.size(); ++i) {
				ArcObject* child = object->childs.at(i);
				drawCurrentObject(child);
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
	drawObjectProperties(selectedObject);
	ImGui::EndChild();
	ImGui::PopStyleVar();
#endif
}

void ArcDebug::drawObjectProperties(ArcObject *obj)
{
	if (obj == nullptr)
		return;
#ifdef ARC_DEBUG
	std::function<void(const std::string&, ImVec4*)> editColor = [] (const std::string& name, ImVec4* color) {
		ImGui::TextUnformatted(name.c_str());

		ImGui::SliderFloat(std::string("##RedOf" + name).c_str(), &color->x, 0.f, 1.f);
		ImGui::SameLine();
		ImGui::TextUnformatted("Red");

		ImGui::SliderFloat(std::string("##GreenOf" + name).c_str(), &color->y, 0.f, 1.f);
		ImGui::SameLine();
		ImGui::TextUnformatted("Green");

		ImGui::SliderFloat(std::string("##BlueOf" + name).c_str(), &color->z, 0.f, 1.f);
		ImGui::SameLine();
		ImGui::TextUnformatted("Blue");

		ImGui::SliderFloat(std::string("##AlphaOf" + name).c_str(), &color->w, 0.f, 1.f);
		ImGui::SameLine();
		ImGui::TextUnformatted("Alpha");

		ImGui::TextUnformatted("Color:");
		ImGui::SameLine();
		ImGui::ColorEdit4(std::string("##" + name).c_str(), &color->x, ImGuiColorEditFlags_NoInputs);
	};

	auto drawInheritObject = [&editColor](ArcEngine::OBJECT_TYPE type, ArcObject *obj) {
		switch (type)
		{
		case ArcEngine::OBJECT:
		{
			ImGui::TextColored(ImVec4(0.3f, 0.3f, 0.8f, 1.f), "%s", typeToName(obj->type()).c_str());
			ImGui::TextUnformatted("Properties");
			bool enabled = obj->isEnabled();
			if (ImGui::Checkbox("Enabled", &enabled)) {
				obj->setEnabled(enabled);
			}
			if (ImGui::Checkbox("Draw rect", &obj->drawDebugRect));
			if (obj->drawDebugRect) {
				{//Color
					ImVec4 color = Utils::convertFromColor(obj->debugRectColor());
					editColor("Debug color", &color);
					obj->setDebugRectColor(Utils::convertToColor(color));
				}
				{//Debug border size
					float lineSize = obj->debugRectLineSize();
					ImGui::DragFloat("##DebugBorderSize", &lineSize);
					ImGui::SameLine();
					ImGui::TextUnformatted("Size");
					obj->setDebugRectLineSize(lineSize);
				}
			}

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				{//Position
					float x = obj->x();
					float y = obj->y();
					ImGui::TextUnformatted("Position");
					ImGui::DragFloat("##PositionX", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##PositionY", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					obj->setPos(sf::Vector2f(x, y));
				}
				{//Origin
					float x = obj->originX();
					float y = obj->originY();
					ImGui::TextUnformatted("Origin");
					ImGui::DragFloat("##OriginX", &x, 0.05f, 0, 1.f);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##OriginY", &y, 0.05f, 0, 1.f);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					obj->setOrigin(sf::Vector2f(x, y));
				}
				{//Size
					float x = obj->width();
					float y = obj->height();
					ImGui::TextUnformatted("Size");
					ImGui::DragFloat("##Width", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("Width");
					ImGui::DragFloat("##Height", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Height");
					obj->setSize(sf::Vector2f(x, y));
				}
				{//Scale
					float x = obj->scaleX();
					float y = obj->scaleY();
					ImGui::TextUnformatted("Scale");
					ImGui::DragFloat("##ScaleX", &x, 0.05f);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##ScaleY", &y, 0.05f);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					obj->setScale(sf::Vector2f(x, y));
				}
				{//Angle
					float angle = obj->rotation();
					ImGui::TextUnformatted("Rotation");
					ImGui::DragFloat("##Angle", &angle);
					ImGui::SameLine();
					ImGui::TextUnformatted("Angle");
					obj->setRotation(angle);
				}
			}
		}
			break;
		case ArcEngine::SPRITE:
		{
			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcSprite *sprite = static_cast<ArcSprite*>(obj);
				{//Color
					ImVec4 color = Utils::convertFromColor(sprite->color());
					editColor("Color", &color);
					sprite->setColor(Utils::convertToColor(color));
				}
				{//Texture ID
					TextureType id = sprite->textureID();
					ImGui::TextUnformatted("Texture ID");
					ImGui::InputInt("##TextureID", &id);
					ImGui::SameLine();
					ImGui::TextUnformatted("ID");
					sprite->setTexture(id);
				}
				{//Border color
					ImVec4 color = Utils::convertFromColor(sprite->borderColor());
					editColor("BorderColor", &color);
					sprite->setBorderColor(Utils::convertToColor(color));
				}
				{//Border size
					float borderSize = sprite->borderSize();
					ImGui::DragFloat("##BorderSize", &borderSize, 1.f, 1);
					ImGui::SameLine();
					ImGui::TextUnformatted("Border Size");
					sprite->setBorderSize(borderSize);
				}
			}
		}
			break;
		case ArcEngine::LABEL:
		{
			if (ImGui::CollapsingHeader("Label", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcLabel *label = static_cast<ArcLabel*>(obj);
				{//Texture ID
					constexpr int BUF_SIZE = 255;
					char buf[BUF_SIZE];
					strcpy_s(buf, label->text().c_str());
					ImGui::TextUnformatted("Text");
					ImGui::InputText("##Text", buf, BUF_SIZE);
					ImGui::SameLine();
					ImGui::TextUnformatted("Text");
					label->setText(std::string(buf));
				}
				{//Autosize
					bool autoSize = label->autoSize();
					ImGui::Checkbox("##AutoSize", &autoSize);
					ImGui::SameLine();
					ImGui::TextUnformatted("AutoSize");
					label->setAutoSize(autoSize);
				}
				ImGui::TextUnformatted("Font");
				{//Font size
					int fontSize = label->fontSize();
					ImGui::DragInt("##FontSize", &fontSize, 1.f, 1);
					ImGui::SameLine();
					ImGui::TextUnformatted("Size");
					label->setFontSize(fontSize);
				}
				{//Font color
					ImVec4 color = Utils::convertFromColor(label->color());
					editColor("Font color", &color);
					label->setColor(Utils::convertToColor(color));
				}
				{//Text border size
					float textBorderSize = label->textBorderSize();
					ImGui::DragFloat("##TextBorderSize", &textBorderSize);
					ImGui::SameLine();
					ImGui::TextUnformatted("Size");
					label->setTextBorderSize(textBorderSize);
				}
				{//Text border color
					ImVec4 color = Utils::convertFromColor(label->textBorderColor());
					editColor("Border color", &color);
					label->setTextBorderColor(Utils::convertToColor(color));
				}
			}
		}
			break;
		case ArcEngine::BUTTON:
		{
			if (ImGui::CollapsingHeader("Button", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcButton *button = static_cast<ArcButton*>(obj);
				{//Clickable
					bool clickable = button->isClickable();
					ImGui::TextUnformatted("Color");
					ImGui::Checkbox("##Clickable", &clickable);
					ImGui::SameLine();
					ImGui::TextUnformatted("Color");
					button->setClickable(clickable);
				}
			}
		}
			break;
		case ArcEngine::ANIMATED_SPRITE:
		{
			if (ImGui::CollapsingHeader("Frame animated sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcAnimatedSprite *sprite = static_cast<ArcAnimatedSprite*>(obj);
				{//Speed
					float speed = sprite->speed();
					ImGui::TextUnformatted("Speed");
					ImGui::DragFloat("##Speed", &speed);
					ImGui::SameLine();
					ImGui::TextUnformatted("Speed");
					sprite->setSpeed(speed);
				}
				{//Frame count
					int frameCount = sprite->frameCount();
					ImGui::TextUnformatted("Frame count");
					ImGui::DragInt("##FrameCount", &frameCount);
					ImGui::SameLine();
					ImGui::TextUnformatted("Count");
					sprite->setFrameCount(frameCount);
				}
				{//Row count
					int rowCount = sprite->rowCount();
					ImGui::TextUnformatted("Row count");
					ImGui::DragInt("##RowCount", &rowCount);
					ImGui::SameLine();
					ImGui::TextUnformatted("Count");
					sprite->setRowCount(rowCount);
				}
				{//Looped
					bool looped = sprite->isLooped();
					ImGui::TextUnformatted("Looped");
					ImGui::Checkbox("##Looped", &looped);
					ImGui::SameLine();
					ImGui::TextUnformatted("Looped");
					sprite->setLooped(looped);
				}
				{//Cycled
					bool cycled = sprite->isCycled();
					ImGui::TextUnformatted("Cycled");
					ImGui::Checkbox("##Cycled", &cycled);
					ImGui::SameLine();
					ImGui::TextUnformatted("Cycled");
					sprite->setCycled(cycled);
				}
			}
		}
			break;
		case ArcEngine::SKELETON_ANIMATION:
		{
			if (ImGui::CollapsingHeader("Skeleton", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcSkeletonAnimation *skeleton = static_cast<ArcSkeletonAnimation*>(obj);
				{//Color
					ImVec4 color = Utils::convertFromColor(skeleton->color());
					editColor("Color", &color);
					skeleton->setColor(Utils::convertToColor(color));
				}
			}
		}
			break;
		case ArcEngine::LAYOUT:
		{
			if (ImGui::CollapsingHeader("Layout", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcLayout *layout = static_cast<ArcLayout*>(obj);
				{//Offset
					float x = layout->offsetX();
					float y = layout->offsetY();
					ImGui::TextUnformatted("Offset");
					ImGui::DragFloat("##OffsetX", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##OffsetY", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					layout->setOffset(x, y);
				}
				{//Grid
					float x = layout->rows();
					float y = layout->columns();
					ImGui::TextUnformatted("Grid");
					ImGui::DragFloat("##Rows", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					ImGui::DragFloat("##Columns", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					layout->setGrid(x, y);
				}
				{//AutoSize
					bool autoSize = layout->autoSize();
					ImGui::Checkbox("##AutoSize", &autoSize);
					ImGui::SameLine();
					ImGui::TextUnformatted("AutoSize");
					layout->setAutoSize(autoSize);
				}
			}
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
	case ArcEngine::ANIMATED_SPRITE:
		drawInheritObject(ArcEngine::SPRITE, obj);
		drawInheritObject(ArcEngine::ANIMATED_SPRITE, obj);
		break;
	case ArcEngine::SKELETON_ANIMATION:
		drawInheritObject(ArcEngine::SKELETON_ANIMATION, obj);
		break;
	case ArcEngine::LAYOUT:
		drawInheritObject(ArcEngine::LAYOUT, obj);
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
	case ArcEngine::ANIMATED_SPRITE:
		return "Animated sprite";
		break;
	case ArcEngine::SKELETON_ANIMATION:
		return "Skeleton animation";
		break;
	case ArcEngine::LAYOUT:
		return "Layout";
		break;
	default:
		break;
	}
	return "Undef";
}
