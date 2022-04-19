#ifndef ARCCHECKBOX_H
#define ARCCHECKBOX_H

#include <arcbutton.h>

class ArcCheckBox : public ArcButton
{
public:
	ArcCheckBox(const std::string& name);

	void setTickTexture(TextureType textureID);
	void setCheckedCallback(const std::function<void(bool)>& callback);
	void setChecked(bool checked);

	void draw(sf::RenderTarget *const target) override;
	void updatePos() override;
	void updateSize() override;
	void updateScale() override;

	void setTexture(TextureType textureID) override;
	void setColor(const sf::Color& color) override;

private:
	sf::RectangleShape tick;
	TextureType m_tickTextureID = -1;
	bool checked = false;
	std::function<void(bool)> m_checkedCallback = nullptr;
	static constexpr float tickSize = 0.6f;
	void initTexture();
};

#endif // ARCCHECKBOX_H
