#include "arcdebug.h"
#include <managers.h>
#include <ArcEngine>
#include <mainwindow.h>
#include <ArcEngineUtils>
#include <collisions.h>
#include <ArcObject>
#include <ArcSprite>
#include <ArcButton>
#include <ArcLabel>
#include <ArcAnimatedSprite>
#include <ArcSkeletonAnimation>
#include <ArcLayout>
#include <ArcPolygon>
#include <ArcRect>
#include <ArcNavigationMap>
#include <ArcLine>
#include <ArcScrollArea>
#include <ArcCheckBox>
#include <ArcLog>
#include "../src/Widgets/customwidgets.h"

#include "imgui.h"
#include "imgui-SFML.h"
#include "imconfig-SFML.h"
#include "imgui_stdlib.h"

ArcObject *ArcDebug::selectedObject = nullptr;

ArcDebug::~ArcDebug()
{
	for(DebugSection *section : sections) {
		delete section;
	}
}

void ArcDebug::setObject(ArcObject *object)
{
	this->object = object;
	selectedObject = object;
}

void ArcDebug::draw(sf::RenderTarget *target)
{
	drawFrame();
#ifdef ARC_DEBUG
//	ImGui::SFML::SetCurrentWindow(*static_cast<sf::RenderWindow*>(target));
	ImGui::SFML::Render(*target);
#endif
}

void ArcDebug::init()
{
	sections.push_back(new NotificationDebug());

	ImGui::SFML::Init(*static_cast<sf::RenderWindow*>(Engine::Instance().window()));
	FPS.updateTimer.setInterval(200);
	FPS.updateTimer.setRepeat(false);
	FPS.updateTimer.start();
}

void ArcDebug::clear()
{
	ImGui::SFML::Shutdown();
}

void ArcDebug::update()
{
	const sf::Time dt = clock.restart();
	ImGui::SFML::Update(*static_cast<sf::RenderWindow*>(Engine::Instance().window()), dt);
	const float currentTime = FPS.clock.restart().asSeconds();
	if (!FPS.updateTimer.isActive()) {
		FPS.value = 1.f / currentTime;
		FPS.updateTimer.start();
	}
}

bool ArcDebug::eventFilter(sf::Event *event)
{
	if (event->type == sf::Event::KeyPressed && event->key.code == sf::Keyboard::F3) {
		visible = !visible;
	}
	if (event->type == sf::Event::MouseMoved) {
		screenPos = sf::Vector2i(event->mouseMove.x, event->mouseMove.y);
		mousePos = PIXEL_TO_POS(screenPos.x, screenPos.y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
		std::function<ArcObject*(ArcObject*, const sf::Vector2f&)> findObject = nullptr;
		findObject = [&findObject](ArcObject* object, const sf::Vector2f& pos) ->ArcObject* {
			for (int i = object->m_childs.size() - 1; i >= 0; --i) {
				ArcObject *child = object->m_childs.at(i);
				if (!child->isEnabled())
					continue;
				ArcObject *findedChild = findObject(child, pos);
				if (findedChild != nullptr)
					return findedChild;
				if (Intersection::contains(child, pos)) {
					return child;
				}
			}
			if (Intersection::contains(object, pos)) {
				return object;
			}
			return nullptr;
		};

		if (event->type == sf::Event::MouseButtonReleased || event->type == sf::Event::MouseButtonPressed) {
			const sf::Vector2f pos = PIXEL_TO_POS(event->mouseButton.x, event->mouseButton.y);
			ArcObject *findedObject = findObject(object, pos);
			if (selectedObject != nullptr) {
				selectedObject = findedObject;
				return true;
			}
		}
		if (event->type == sf::Event::MouseMoved) {
			ArcObject *findedObject = findObject(object, mousePos);
			static ArcObject* lastFinded = nullptr;
			if (findedObject != nullptr) {
				if (lastFinded != nullptr)
					lastFinded->drawDebugRect = false;
				findedObject->drawDebugRect = true;
				lastFinded = findedObject;
			}
		}
	}
	ImGui::SFML::ProcessEvent(*static_cast<sf::RenderWindow*>(Engine::Instance().window()), *event);
	ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse || io.WantCaptureKeyboard;
}

void ArcDebug::addSection(DebugSection *section)
{
	sections.push_back(section);
}

void ArcDebug::removeSection(DebugSection *section)
{
	if (auto it = std::find(sections.begin(), sections.end(), section); it != sections.end()) {
		sections.erase(it);
	}
}

void ArcDebug::drawFrame()
{
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
				const float fpsColorV = FPS.value * ArcEngine::FRAME_TIME;
				ImVec4 fpsColor;
				fpsColor.w = 1.f;
				fpsColor.x = 1.f - fpsColorV;
				fpsColor.y = fpsColorV;
				fpsColor.z = 0;
				ImGui::TextColored(fpsColor, "%s", std::to_string(static_cast<int>(FPS.value)).c_str());

				if (ImGui::CollapsingHeader("Coordinates", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::TextUnformatted("Resolution:");
					ImGui::SameLine();
					ImGui::TextUnformatted(std::string(std::to_string(static_cast<int>(RESOLUTIONF.x)) + " : " +
													   std::to_string(static_cast<int>(RESOLUTIONF.y))).c_str());

					ImGui::TextUnformatted("Screen coordinates:");
					ImGui::SameLine();
					ImGui::TextUnformatted(std::string(std::to_string(screenPos.x) + " : " +
													   std::to_string(screenPos.y)).c_str());

					ImGui::TextUnformatted("Game coordinates:");
					ImGui::SameLine();
					ImGui::TextUnformatted(std::string(ArcEngine::to_string_with_precision(mousePos.x, 2) + " : " +
													   ArcEngine::to_string_with_precision(mousePos.y, 2)).c_str());
				}
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
			for(DebugSection *section : sections) {
				if (ImGui::BeginTabItem(section->name().c_str())) {
					section->draw();
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
//	ImGui::ShowDemoWindow();
}

void ArcDebug::drawObject(ArcObject *obj)
{
	if (obj == nullptr)
		return;
	const float height = ImGui::GetWindowSize().y;
	static float w = 200.0f;
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
	ImGui::BeginChild("Objects tree", ImVec2(w, 0), true);
	ImGui::TextUnformatted("Objects tree");
	std::function<void(ArcObject *)> drawCurrentObject = nullptr;
	drawCurrentObject = [&drawCurrentObject](ArcObject *object)-> void {
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool isSelected = selectedObject == object;
		if (isSelected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		if (object->hasChild(selectedObject))
			ImGui::SetNextItemOpen(true);

		bool opened = ImGui::TreeNodeEx(object->name().c_str(), node_flags);
		if (ImGui::IsItemClicked()) {
			selectedObject = object;
		}

		if (opened) {
			for (unsigned i = 0; i < object->m_childs.size(); ++i) {
				ArcObject* child = object->m_childs.at(i);
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
	if (!selectedObject->isDestroyed())
		drawObjectProperties(selectedObject);
	ImGui::EndChild();
	ImGui::PopStyleVar();
}

void ArcDebug::drawObjectProperties(ArcObject *obj)
{
	if (obj == nullptr)
		return;
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
			ImGui::TextColored(ArcEngine::convertFromColor(typeToColor(obj->type())), "%s", typeToName(obj->type()).c_str());
			ImGui::TextUnformatted(obj->path().c_str());
			ImGui::TextUnformatted("Properties");
			bool enabled = obj->isEnabled();
			if (ImGui::Checkbox("Enabled", &enabled)) {
				obj->setEnabled(enabled);
			}
			if (ImGui::Checkbox("Draw rect", &obj->drawDebugRect));
			if (obj->drawDebugRect) {
				{//Color
					ImVec4 color = ArcEngine::convertFromColor(obj->debugRectColor());
					editColor("Debug color", &color);
					obj->setDebugRectColor(ArcEngine::convertToColor(color));
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
					sf::Vector2f pos = obj->pos();
					ImGui::TextUnformatted("Position");
					ImGui::DragFloat("##PositionX", &pos.x);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##PositionY", &pos.y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					if (pos.x != obj->x() || pos.y != obj->y())
						obj->setPos(pos);
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
					if (x != obj->originX() || y != obj->originY())
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
					if (x != obj->width() || y != obj->height())
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
					if (x != obj->scaleX() || y != obj->scaleY())
						obj->setScale(sf::Vector2f(x, y));
				}
				{//Angle
					float angle = obj->rotation();
					ImGui::TextUnformatted("Rotation");
					ImGui::DragFloat("##Angle", &angle, 1.f, -360.f, 360.f);
					ImGui::SameLine();
					ImGui::TextUnformatted("Angle");
					if (angle != obj->rotation())
						obj->setRotation(angle);
				}
				{//Alpha
					float alpha = obj->alpha();
					ImGui::TextUnformatted("Opacity");
					ImGui::DragFloat("##Alpha", &alpha, 0.01f, 0, 1.f);
					ImGui::SameLine();
					ImGui::TextUnformatted("Alpha");
					if (alpha != obj->alpha())
						obj->setAlpha(alpha);
				}
				{//DragEnabled
					bool isDragEnabled = obj->isDragEnabled();
					ImGui::Checkbox("##DragEnabled", &isDragEnabled);
					ImGui::SameLine();
					ImGui::TextUnformatted("DragEnabled");
					obj->setDragEnabled(isDragEnabled);
				}
			}
		}
			break;
		case ArcEngine::SPRITE:
		{
			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcSprite *sprite = static_cast<ArcSprite*>(obj);
				{//Color
					ImVec4 color = ArcEngine::convertFromColor(sprite->color());
					editColor("Color", &color);
					sprite->setColor(ArcEngine::convertToColor(color));
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
					ImVec4 color = ArcEngine::convertFromColor(sprite->borderColor());
					editColor("BorderColor", &color);
					sprite->setBorderColor(ArcEngine::convertToColor(color));
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
				{//Text
					std::string buf = label->text().toAnsiString();
					ImGui::TextUnformatted("Text");
					ImGui::InputText("##Text", &buf);
					ImGui::SameLine();
					ImGui::TextUnformatted("Text");
					label->setText(buf);
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
					ImVec4 color = ArcEngine::convertFromColor(label->color());
					editColor("Font color", &color);
					label->setColor(ArcEngine::convertToColor(color));
				}
				{//Text border size
					float textBorderSize = label->borderSize();
					ImGui::DragFloat("##TextBorderSize", &textBorderSize);
					ImGui::SameLine();
					ImGui::TextUnformatted("Size");
					label->setBorderSize(textBorderSize);
				}
				{//Text border color
					ImVec4 color = ArcEngine::convertFromColor(label->borderColor());
					editColor("Border color", &color);
					label->setBorderColor(ArcEngine::convertToColor(color));
				}
				{//Align
					float x = label->align().x;
					float y = label->align().y;
					ImGui::TextUnformatted("ALign");
					ImGui::DragFloat("##AlignX", &x, 0.01f, 0.f, 1.f);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##AlignY", &y, 0.01f, 0.f, 1.f);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					if (x != label->align().x || y != label->align().y)
						label->setAlign(sf::Vector2f(x, y));
				}
				{//Multiline
					int maxLines = label->maxLines();
					ImGui::DragInt("##MaxLines", &maxLines, 1.f, 1, 9);
					ImGui::SameLine();
					ImGui::TextUnformatted("Max lines");
					label->setMaxLines(maxLines);
				}
				if (ImGui::Checkbox("Show label debug", &label->showLabelDebug)) {

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
					ImGui::TextUnformatted("Clickable");
					ImGui::Checkbox("##Clickable", &clickable);
					ImGui::SameLine();
					ImGui::TextUnformatted("Clickable");
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
					int speed = sprite->speed();
					ImGui::TextUnformatted("Speed");
					ImGui::DragInt("##Speed", &speed);
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
					ImVec4 color = ArcEngine::convertFromColor(skeleton->color());
					editColor("Color", &color);
					skeleton->setColor(ArcEngine::convertToColor(color));
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
					ImGui::DragFloat("##Columns", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("Columns");
					ImGui::DragFloat("##Rows", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Rows");
					layout->setGrid(x, y);
				}
				{//AutoSize
					bool autoSize = layout->autoSize();
					ImGui::Checkbox("##AutoSize", &autoSize);
					ImGui::SameLine();
					ImGui::TextUnformatted("AutoSize");
					layout->setAutoSize(autoSize);
				}
				{//Border offser
					float x = layout->borderOffset().x;
					float y = layout->borderOffset().y;
					ImGui::TextUnformatted("Border offseе");
					ImGui::DragFloat("##BorderOffsetX", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##BorderOffsetY", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					layout->setBorderOffset(x, y);
				}
				{//update
					if (ImGui::Button("updateLayout")) {
						layout->updateLayout();
					}
				}
			}
		}
			break;
		case ArcEngine::POLYGON:
		{
			if (ImGui::CollapsingHeader("Polygon", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcPolygon *polygon = static_cast<ArcPolygon*>(obj);
				{//Color
					ImVec4 color = ArcEngine::convertFromColor(polygon->color());
					editColor("Color", &color);
					polygon->setColor(ArcEngine::convertToColor(color));
				}
				{//Border color
					ImVec4 color = ArcEngine::convertFromColor(polygon->borderColor());
					editColor("BorderColor", &color);
					polygon->setBorderColor(ArcEngine::convertToColor(color));
				}
				{//Border size
					float borderSize = polygon->borderSize();
					ImGui::DragFloat("##BorderSize", &borderSize, 1.f, 1);
					ImGui::SameLine();
					ImGui::TextUnformatted("Border Size");
					polygon->setBorderSize(borderSize);
				}
			}
		}
			break;
		case ArcEngine::RECT:
		{
			if (ImGui::CollapsingHeader("Rectangle", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcRect *rect = static_cast<ArcRect*>(obj);
				{//Color
					ImVec4 color = ArcEngine::convertFromColor(rect->color());
					editColor("Color", &color);
					rect->setColor(ArcEngine::convertToColor(color));
				}
				{//Border color
					ImVec4 color = ArcEngine::convertFromColor(rect->borderColor());
					editColor("BorderColor", &color);
					rect->setBorderColor(ArcEngine::convertToColor(color));
				}
				{//Border size
					float borderSize = rect->borderSize();
					ImGui::DragFloat("##BorderSize", &borderSize, 1.f, 1);
					ImGui::SameLine();
					ImGui::TextUnformatted("Border Size");
					rect->setBorderSize(borderSize);
				}
			}
		}
			break;
		case ArcEngine::NAVIGATION_MAP:
		{
			if (ImGui::CollapsingHeader("Navigation map", ImGuiTreeNodeFlags_DefaultOpen)) {
				NavigationMap *navMap = static_cast<NavigationMap*>(obj);
				if (ImGui::Checkbox("Show", &navMap->debug)) {

				}
				{//cells
					int cells = navMap->cells();
					ImGui::DragInt("##Cells", &cells, 1.f, 0, 1000);
					ImGui::SameLine();
					ImGui::TextUnformatted("Cells");
					navMap->setCells(cells);
				}
			}
		}
			break;
		case ArcEngine::PATH:
		{
			if (ImGui::CollapsingHeader("Way points", ImGuiTreeNodeFlags_DefaultOpen)) {
				PathObject *pathObj = static_cast<PathObject*>(obj);
				if (ImGui::Checkbox("Show", &pathObj->m_debug)) {

				}
				{//Radius
					ImGui::DragFloat("##Radius", &pathObj->radius, 1.f, 1.f, 100.f);
					ImGui::SameLine();
					ImGui::TextUnformatted("Radius");
				}
				{//Lines color
					ImVec4 color = ArcEngine::convertFromColor(pathObj->m_color);
					editColor("Lines color", &color);
					obj->setDebugRectColor(ArcEngine::convertToColor(color));
				}
				{//Points color
					ImVec4 color = ArcEngine::convertFromColor(pathObj->m_pointColor);
					editColor("Points color", &color);
					obj->setDebugRectColor(ArcEngine::convertToColor(color));
				}
				pathObj->debug();
			}
		}
			break;
		case ArcEngine::SCROLL_AREA:
		{
			if (ImGui::CollapsingHeader("Scroll Area", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcScrollArea *scrollArea = static_cast<ArcScrollArea*>(obj);
				{//isNeedRepaint
					bool repaint = scrollArea->isNeedRepaint();
					ImGui::Checkbox("##isNeedRepaint", &repaint);
					ImGui::SameLine();
					ImGui::TextUnformatted("Need Repaint");
					scrollArea->setNeedRepaint(repaint);
				}
			}
		}
			break;
		case ArcEngine::CHECKBOX:
		{
			if (ImGui::CollapsingHeader("Checkbox", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcCheckBox *checkbox = static_cast<ArcCheckBox*>(obj);
				{//Checked
					bool checked = checkbox->isChecked();
					ImGui::Checkbox("##isChecked", &checked);
					ImGui::SameLine();
					ImGui::TextUnformatted("Checked");
					checkbox->setChecked(checked);
				}
			}
		}
			break;
		case ArcEngine::ZOOM_VIEW:
		{
			if (ImGui::CollapsingHeader("ZoomView", ImGuiTreeNodeFlags_DefaultOpen)) {
				ZoomView *zoomView = static_cast<ZoomView*>(obj);
				{//View pos
					sf::Vector2f pos = zoomView->viewPos();
					ImGui::TextUnformatted("View position");
					ImGui::DragFloat("##ViewPosX", &pos.x);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##ViewPosY", &pos.y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					if (pos.x != obj->x() || pos.y != obj->y())
						zoomView->setView(pos);
				}
			}
		}
			break;
		case ArcEngine::LINE:
		{
			if (ImGui::CollapsingHeader("Line", ImGuiTreeNodeFlags_DefaultOpen)) {
				ArcLine *line = static_cast<ArcLine*>(obj);
				{//Color
					ImVec4 color = ArcEngine::convertFromColor(line->color());
					editColor("Color", &color);
					line->setColor(ArcEngine::convertToColor(color));
				}
				{//First point
					float x = line->firstPoint().x;
					float y = line->firstPoint().y;
					ImGui::TextUnformatted("FirstPoint");
					ImGui::DragFloat("##FirstPointX", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##FirstPointY", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					if (x != line->firstPoint().x || y != line->firstPoint().y)
						line->setFirstPoint(sf::Vector2f(x, y));
				}
				{//Second point
					float x = line->secondPoint().x;
					float y = line->secondPoint().y;
					ImGui::TextUnformatted("SecondPoint");
					ImGui::DragFloat("##SecondPointX", &x);
					ImGui::SameLine();
					ImGui::TextUnformatted("X");
					ImGui::DragFloat("##SecondPointY", &y);
					ImGui::SameLine();
					ImGui::TextUnformatted("Y");
					if (x != line->secondPoint().x || y != line->secondPoint().y)
						line->setSecondPoint(sf::Vector2f(x, y));
				}
				{//Line Width
					float width = line->lineWidth();
					ImGui::TextUnformatted("Line width");
					ImGui::DragFloat("##LineWidth", &width);
					ImGui::TextUnformatted("Width");
					line->setLineWidth(width);
				}
				//updateTarget
				if (ImGui::Button("updateTarget")) {
					line->updateTarget();
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
	case ArcEngine::BUTTON:
		drawInheritObject(ArcEngine::SPRITE, obj);
		break;
	case ArcEngine::ANIMATED_SPRITE:
		drawInheritObject(ArcEngine::SPRITE, obj);
		break;
	case ArcEngine::CHECKBOX:
		drawInheritObject(ArcEngine::SPRITE, obj);
		drawInheritObject(ArcEngine::BUTTON, obj);
		break;
	default:
		break;
	}
	if (obj->type() != ArcEngine::OBJECT)
		drawInheritObject(obj->type(), obj);
	drawCustomData(obj);
}

void ArcDebug::drawCustomData(ArcObject *obj)
{
	auto drawName = [](const std::string& name) {
		ImGui::SameLine();
		ImGui::TextUnformatted(name.c_str());
	};
	if (ImGui::CollapsingHeader("Custom data", ImGuiTreeNodeFlags_DefaultOpen)) {
		for(const auto& [name, value] : obj->m_data) {
			switch (value.type())
			{
			case ArcVariant::BOOLEAN:
			{
				bool v = value.toBool();
				ImGui::Checkbox(std::string("##" + name).c_str(), &v);
				drawName(name);
				if (v != value.toBool())
					obj->setData(name, v);
			}
				break;
			case ArcVariant::DOUBLE:
			case ArcVariant::FLOAT:
			{
				float v = value.toFloat();
				ImGui::DragFloat(std::string("##" + name).c_str(), &v);
				drawName(name);
				if (v != value.toFloat())
					obj->setData(name, v);
			}
				break;
			case ArcVariant::UINT:
			case ArcVariant::INT:
			{
				int v = value.toInt();
				ImGui::DragInt(std::string("##" + name).c_str(), &v);
				drawName(name);
				if (v != value.toInt())
					obj->setData(name, v);
			}
				break;
			case ArcVariant::STRING:
			{
				std::string v = value.toString();
				ImGui::InputText(std::string("##" + name).c_str(), &v);
				drawName(name);
				if (v != value.toString())
					obj->setData(name, v);
			}
				break;
			default:
				break;
			}
		}
	}
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
	case ArcEngine::POLYGON:
		return "Polygon";
		break;
	case ArcEngine::RECT:
		return "Rectangle";
		break;
	case ArcEngine::NAVIGATION_MAP:
		return "Navigation map";
		break;
	case ArcEngine::PATH:
		return "Way points";
		break;
	case ArcEngine::CHECKBOX:
		return "Checkbox";
		break;
	case ArcEngine::SCROLL_AREA:
		return "Scroll area";
		break;
	case ArcEngine::ZOOM_VIEW:
		return "Zoom view";
		break;
	case ArcEngine::LINE:
		return "Line";
		break;
	default:
		break;
	}
	return "Undef";
}

sf::Color ArcDebug::typeToColor(ArcEngine::OBJECT_TYPE type)
{
	switch (type)
	{
	case ArcEngine::OBJECT:
		return sf::Color(0, 160, 176, 255);
		break;
	case ArcEngine::SPRITE:
		return sf::Color(0, 77, 84, 255);
		break;
	case ArcEngine::LAYOUT:
		return sf::Color(113, 157, 189, 255);
		break;
	default:
		break;
	}
	return sf::Color(186,101,30, 255);
}



DebugSection::DebugSection(const std::string &name)
	: m_name(name)
{

}

void DebugSection::draw()
{

}

std::string DebugSection::name() const
{
	return m_name;
}

NotificationDebug::NotificationDebug()
	: DebugSection("Notifications")
{
	Engine::Instance().getOptions()->addNotificationCallback([this](const std::string &name,
															 ArcObject *object,
															 const std::vector<ArcVariant>& args) {
		std::string objectStr;
		std::string objectPathStr;
		std::string argsStr;
		if (object == nullptr) {
			objectStr = "null";
			objectPathStr = "-";
		}
		else {
			objectStr = object->name();
			objectPathStr = object->path();
		}
		if (!args.empty()) {
			StringLog log;
			log << args;
			argsStr = log.toString();
		}
		notifications.push_back(std::make_tuple(name, objectStr, objectPathStr, argsStr));
	});
}

void NotificationDebug::draw()
{
	enum COLUMNS {
		NOTIFICATION_TYPE,
		OBJECT_NAME,
		PATH,
		ARGS,
		COLUMNS_COUNT
	};
	std::vector<std::string> headers = {
		"Type", "Object", "Path", "Args"
	};

	if (ImGui::BeginTable("##Notifications", COLUMNS::COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
		ImGui::TableSetupColumn(headers.at(COLUMNS::NOTIFICATION_TYPE).c_str());
		ImGui::TableSetupColumn(headers.at(COLUMNS::OBJECT_NAME).c_str());
		ImGui::TableSetupColumn(headers.at(COLUMNS::PATH).c_str());
		ImGui::TableSetupColumn(headers.at(COLUMNS::ARGS).c_str());
		ImGui::TableHeadersRow();

		for (unsigned row = notifications.size() - 1; row != 0; --row) {
			ImGui::TableNextRow();
			for (int column = 0; column < COLUMNS::COLUMNS_COUNT; column++) {
				ImGui::TableSetColumnIndex(column);
				const std::tuple<std::string, std::string, std::string, std::string> data = notifications.at(row);
				switch (column)
				{
				case COLUMNS::NOTIFICATION_TYPE:
					ImGui::Selectable(std::get<NOTIFICATION_TYPE>(data).c_str());
					break;
				case COLUMNS::OBJECT_NAME:
					ImGui::Selectable(std::get<OBJECT_NAME>(data).c_str());
					break;
				case COLUMNS::PATH:
					ImGui::Selectable(std::get<PATH>(data).c_str());
					break;
				case COLUMNS::ARGS:
					ImGui::Selectable(std::get<ARGS>(data).c_str());
					break;
				default:
					break;
				}
			}
		}
		ImGui::EndTable();
	}
}
