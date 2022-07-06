#ifndef ARCWINDOW_H
#define ARCWINDOW_H

#include <arcsprite.h>

class ArcWindow : public ArcSprite
{
public:
	ArcWindow(const std::string& name = "Window");
	~ArcWindow() override;

	virtual void init();
	virtual void deinit();

	bool eventFilter(sf::Event *event) override;

	void setModal(bool modal);
	bool isModal() const;

	void close();

	WindowType type() const;
	void setType(const WindowType &type);

protected:

private:
	WindowType m_type;
	bool modal = false;
};

#endif // ARCWINDOW_H
