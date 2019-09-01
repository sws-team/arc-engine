#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "stdheader.h"
#include "controlsettings.h"
#include <sstream>

class GlobalVariables
{
public:
    static GlobalVariables &Instance();

    Font &font();

    String applicationPath() const;
    void setApplicationPath(const String &applicationPath);

	static constexpr float FRAME_TIME = 1.0f / 60.0f;

    void saveGameSettings();
    void loadGameSettings();

	bool getControl() const;
	void setControl(bool moving);

	ControlSettings controls() const;

	void loadControls();

	const static Color GrayColor;

	template <typename T>
	static std::string to_string_with_precision(const T a_value, const int n = 6)
	{
		std::ostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return out.str();
	}

	Vector2f tileSize() const;
	Vector2f mapTileSize() const;
	constexpr static const int CELL_SIZE = 64;
	constexpr static const int MAP_CELL_SIZE = 32;

private:
	GlobalVariables();
	GlobalVariables(const GlobalVariables& root) = delete;
    GlobalVariables& operator=(const GlobalVariables&) = delete;

    Font m_font;
	String m_applicationPath;
    Vector2i getScreenResolution() const;
	bool m_control;
	ControlSettings m_controls;

	void createDefaultControls();
};

#endif // GLOBALVARIABLES_H
