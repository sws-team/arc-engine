#ifndef SHOPLOCATION_H
#define SHOPLOCATION_H

#include "statewindow.h"
#include "graphics.h"

#include "Game/Characters/charactertypes.h"

class ShopLocation : public StateWindow
{
public:
	ShopLocation();

protected:
	void paint(RenderWindow *window) override;
	void eventFilter(Event *event) override;
	void back() override;
private:
	RectangleShape exitRect;

	RectangleShape slotMachineRect;


	enum SHOP_STATES
	{
		DEFAULT_STATE,
		SLOT_MACHINE,
		DIALOG
	};

	void setState(SHOP_STATES state);
	SHOP_STATES m_state;
	GraphicImage slotMachine;
	Texture slotMachineTexture;

	Sprite first;
	Sprite last;
	Sprite mid;

	void runSlotMachine();

	constexpr static int VARIANTS_COUNT = 16;

	void chectResults();

	enum QuestionType
	{
		Default_question,
		Hiring,
		Money,
		Mission,
		Say,


		HiringForMoney,
		HiringForMoneyYes,
		HiringForMoneyNo,

		HiringKick,
		HiringKickYes,
		HiringKickNo,

		HiringSay,
		HiringSayYes,
		HiringSayNo,

		MissionForMoney,
		MissionForMoneyYes,
		MissionForMoneyNo,
	};

	enum FeelingTypes
	{
		Undefined,
		Hard_not,
		Not,
		May_be,
		Yes,
		Absolutly
	};

	struct Personality
	{
		struct Relation
		{//0-10
			int min;
			int max;
		};

	struct Answer
	{
		vector<string> text;
		QuestionType action;
	};

	map<CHARACTERS, Relation> relations;

	map<QuestionType, map<FeelingTypes, Answer>> answers;
	map<QuestionType, vector<string>> questions;

	int mission_id;
	float money;
	};

	FeelingTypes checkAnswer(const QuestionType &type);
	void resolveAction(QuestionType action);

	GraphicImage dialogPanel;

	struct Dialogs
	{
		Text text;
		QuestionType type;
	};
	vector<Dialogs> dialogs;

	RectangleShape characterRect;
	int currentAnswer;

	map<CHARACTERS, Personality> chars;

	GraphicImage chooseAnswer;

	class Character *currentCharacter;
	void fillPersonalities();

	Dialogs getDialog(const vector<string>& questions, QuestionType type);

	void updateCurrentChoose();

	class Character *interlocutor;

	Text answerText;

	enum DIALOG_STATE
	{
		PLAYER_ASK,
		INTERLOCUTOR_ASK,
		INTERLOCUTOR_ANSWER
	};
	DIALOG_STATE dialogState;

	void fillDialogs(const vector<QuestionType>& questions);
	vector<QuestionType> getQuestions(QuestionType question);

	constexpr static int GLOBAL_OFFSET = 20;

	QuestionType updateQuestionType(QuestionType type) const;
};

#endif // SHOPLOCATION_H
