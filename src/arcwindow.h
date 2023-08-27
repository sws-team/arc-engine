#ifndef ARCWINDOW_H
#define ARCWINDOW_H

#include <ArcSprite>

class ArcWindow : public ArcSprite
{
public:
	ArcWindow(const std::string& name = "Window");
	~ArcWindow() override;

	virtual void init();
	virtual void deinit();

	bool eventFilter(sf::Event *event) override;

	void setModal(bool modal);
	void setUnique(bool unique);
	void setEscCloseable(bool close);
	bool isModal() const;
	bool isUnique() const;
	bool isEscCloseable() const;

	void close();

	WindowType type() const;
	void setType(const WindowType &type);

protected:
	template<class T> static ArcWindow* create() {
		return new T();
	}
private:
	friend class WindowsManager;
	WindowType m_type;
	bool modal = false;
	bool unique = true;
	bool canEscClose = true;
};

#endif // ARCWINDOW_H
