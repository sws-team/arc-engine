#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include "stdheader.h"
#include "controlsettings.h"

class GlobalVariables
{
public:
    static GlobalVariables &Instance();

    Font &font();

    String applicationPath() const;
    void setApplicationPath(const String &applicationPath);

	constexpr static const float CELL_WIDTH = 128;	
	constexpr static const float SCALED_ICON_SIZE = 48;
	constexpr static const float SCALE_ICON_RATE = SCALED_ICON_SIZE / CELL_WIDTH;
	static constexpr size_t MAX_CHARACTERS = 3;
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

	Vector2i tileSize() const;
	void setTileSize(const Vector2i &tileSize);

private:
	GlobalVariables();
	GlobalVariables(const GlobalVariables& root) = delete;
    GlobalVariables& operator=(const GlobalVariables&) = delete;

    Font m_font;
	String m_applicationPath;
    Vector2i getScreenResolution() const;
	bool m_control;
	ControlSettings m_controls;

	Vector2i m_tileSize;

	void createDefaultControls();
};

#endif // GLOBALVARIABLES_H
