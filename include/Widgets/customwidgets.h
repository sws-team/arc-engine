#ifndef CUSTOMWIDGETS_H
#define CUSTOMWIDGETS_H

#include "arcobject.h"

class CircleScale : public ArcObject
{
public:
	CircleScale(const std::string& name, TextureType texture, unsigned sectors = 100);

	unsigned value() const;
	void setValue(unsigned value);
protected:
	void initTransform() override;
private:
	unsigned m_value = 50;
	unsigned m_sectors = 100;
};

class CustomWidgets
{
public:
	static ArcObject* createLoadingIcon();
private:
	CustomWidgets();
};

#endif // CUSTOMWIDGETS_H
