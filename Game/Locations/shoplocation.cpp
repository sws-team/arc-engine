#include "shoplocation.h"
#include "settings.h"
#include "globalvariables.h"
#include "Game/Characters/character.h"
#include "Game/gamecontroller.h"
#include "Game/SpaceShip/spaceship.h"
#include "Game/objectsfactory.h"

const Vector2f elementSize = Vector2f(130, 108);

ShopLocation::ShopLocation()
	: StateWindow()
	,interlocutor(nullptr)
{
	m_state = DEFAULT_STATE;
	setBackground(RESOURCES::SHOP_LOCATION_BACKGROUND);


	exitRect.setSize(Vector2f(500, 350));

	exitRect.setPosition(0.72f * static_cast<float>(Settings::Instance().getResolution().x),
						 0.138f * static_cast<float>(Settings::Instance().getResolution().y));

	exitRect.setFillColor(sf::Color(150, 50, 250));
	exitRect.setOutlineThickness(10);
	exitRect.setOutlineColor(sf::Color(250, 150, 100));



	slotMachineRect.setSize(Vector2f(100, 200));

	slotMachineRect.setPosition(0.1f * static_cast<float>(Settings::Instance().getResolution().x),
						 0.138f * static_cast<float>(Settings::Instance().getResolution().y));

	slotMachineRect.setFillColor(sf::Color(75, 150, 200));
	slotMachineRect.setOutlineThickness(10);
	slotMachineRect.setOutlineColor(sf::Color(100, 250, 100));

	slotMachine.texture.loadFromFile("images/Locations/azino.png");
	slotMachine.sprite.setTexture(slotMachine.texture);
	slotMachine.sprite.setPosition(100, 100);
	slotMachine.sprite.scale(0.5, 0.5);

	slotMachineTexture.loadFromFile("images/Locations/slot_machine.png");

	first.setTexture(slotMachineTexture);
	mid.setTexture(slotMachineTexture);
	last.setTexture(slotMachineTexture);
	runSlotMachine();


	first.setPosition(slotMachine.sprite.getPosition() + Vector2f(209 * 0.5, 409 * 0.5));
	mid.setPosition(first.getPosition() + Vector2f(elementSize.x, 0));
	last.setPosition(mid.getPosition() + Vector2f(elementSize.x, 0));
//	209, 409

	dialogPanel.texture.loadFromFile("images/Locations/dialog.png");
	dialogPanel.sprite.setTexture(dialogPanel.texture);
	dialogPanel.sprite.setPosition(0, (Settings::Instance().getResolution().y) * Settings::Instance().getScaleFactor().y);

	characterRect.setSize(Vector2f(100, 200));

	characterRect.setPosition(0.1f * static_cast<float>(Settings::Instance().getResolution().x),
						 0.7f * static_cast<float>(Settings::Instance().getResolution().y));

	characterRect.setFillColor(Color(0, 76, 100));
	characterRect.setOutlineThickness(10);
	characterRect.setOutlineColor(sf::Color(100, 20, 80));

	answerText.setFont(GlobalVariables::Instance().font());
	answerText.setFillColor(Color::Red);

	chooseAnswer.texture.loadFromFile("images/Locations/choose.png");
	chooseAnswer.sprite.setTexture(chooseAnswer.texture);

	fillPersonalities();
}

void ShopLocation::paint(RenderWindow *window)
{
	switch (m_state)
	{
	case DEFAULT_STATE:
	{
		drawBackground(window);
		window->draw(exitRect);
		window->draw(slotMachineRect);
		window->draw(characterRect);
	}
		break;
	case SLOT_MACHINE:
	{
		window->draw(slotMachine.sprite);
		window->draw(first);
		window->draw(mid);
		window->draw(last);
	}
		break;
	case DIALOG:
	{
		window->draw(dialogPanel.sprite);
		switch (dialogState)
		{
		case PLAYER_ASK:
		{
			window->draw(currentCharacter->getIcon());
			for(const Dialogs& dlg : dialogs)
				window->draw(dlg.text);
			window->draw(chooseAnswer.sprite);
		}
			break;
		case INTERLOCUTOR_ASK:
		case INTERLOCUTOR_ANSWER:
		{
			window->draw(answerText);
			window->draw(interlocutor->getIcon());
		}
			break;
		}
	}
		break;
	}
}

void ShopLocation::eventFilter(Event *event)
{
	switch (m_state)
	{
	case DEFAULT_STATE:
	{
		if (event->type == Event::MouseButtonPressed)
		{
			if (exitRect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				Engine::Instance().setState(Engine::MAIN_LOCATION);
			}
			if (slotMachineRect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				setState(SLOT_MACHINE);
			}
			if (characterRect.getGlobalBounds().contains(event->mouseButton.x, event->mouseButton.y))
			{
				if (interlocutor != nullptr)
					delete interlocutor;

				currentCharacter = GameController::Instance().players().at(0)->getCharacters().at(0);

				interlocutor = ObjectsFactory::Instance().createCharacter(REPAIR_CHARACTER);
//				interlocutor->getIcon().setPosition(dialogPanel.sprite.getGlobalBounds().width - interlocutor->getIcon().getGlobalBounds().width,
//													currentCharacter->getIcon().getPosition().y);

				const float offset = GLOBAL_OFFSET * Settings::Instance().getScaleFactor().y;
				float xPos = dialogPanel.sprite.getPosition().x + offset;
				float yPos = dialogPanel.sprite.getPosition().y + offset;
				answerText.setPosition(xPos, yPos);
				xPos = Settings::Instance().getResolution().x - offset - 128;
				interlocutor->getIcon().setPosition(xPos, yPos);

				fillDialogs(getQuestions(Default_question));

				updateCurrentChoose();

				dialogState = PLAYER_ASK;
				setState(DIALOG);
			}
		}
	}
		break;
	case SLOT_MACHINE:
	{
		if (event->type == Event::KeyPressed)
		{
			if (event->key.code == Keyboard::Space)
			{
				runSlotMachine();
			}
		}
	}
		break;
	case DIALOG:
	{
		if (event->type == Event::KeyPressed)
		{
			switch (event->key.code)
			{
			case Keyboard::Space:
			{
				switch (dialogState)
				{
				case PLAYER_ASK:
				{
					const QuestionType type = dialogs.at(currentAnswer).type;
cout << "question "<<type << endl;
					const FeelingTypes atype = checkAnswer(type);
cout << "feeling "<<atype << endl;
					const Personality personality = chars.at(interlocutor->getType());
					const Personality::Answer answer = personality.answers.at(type).at(atype);
cout << "answer "<<answer.action << endl;
					const vector<string> answers = answer.text;
					const unsigned int n = rand() % answers.size();
cout << "n "<< n << endl;
					answerText.setString(answer.text.at(n));

cout << answer.text.at(n) << endl;

					if (atype == May_be)
					{
						const QuestionType qtype = updateQuestionType(answer.action);
cout << "may be "<< qtype << endl;

						const vector<string> questions = personality.questions.at(qtype);
						const int qn = rand() % questions.size();
						answerText.setString(questions.at(qn));

						fillDialogs(getQuestions(qtype));
cout << "filled" << endl;
						dialogState = INTERLOCUTOR_ASK;
					}
					else
					{
						dialogState = INTERLOCUTOR_ANSWER;
					}
				}
					break;
				case INTERLOCUTOR_ASK:
				{
					dialogState = PLAYER_ASK;
				}
					break;
				case INTERLOCUTOR_ANSWER:
				{
//					resolveAction(answer.action);
					setState(DEFAULT_STATE);
				}
					break;
				}
			}
				break;
			case Keyboard::Down:
			{
				currentAnswer++;
				if (currentAnswer >= dialogs.size())
					currentAnswer = dialogs.size() - 1;
				updateCurrentChoose();
			}
				break;
			case Keyboard::Up:
			{
				currentAnswer--;
				if (currentAnswer < 0)
					currentAnswer = 0;
				updateCurrentChoose();
			}
				break;
			default:
				break;
			}
		}
	}
		break;
	}

	StateWindow::eventFilter(event);
}

void ShopLocation::back()
{
	switch (m_state)
	{
	case DEFAULT_STATE:
	{
		Engine::Instance().setState(Engine::MAIN_LOCATION);
	}
		break;
	case SLOT_MACHINE:
	{
		setState(DEFAULT_STATE);
	}
		break;
	}
}

void ShopLocation::setState(ShopLocation::SHOP_STATES state)
{
	m_state = state;
}

void ShopLocation::runSlotMachine()
{
	const int firstNum = rand() % VARIANTS_COUNT;
	const int midNum = rand() % VARIANTS_COUNT;
	const int lastNum = rand() % VARIANTS_COUNT;

	first.setTextureRect(IntRect(firstNum * elementSize.x, 0, elementSize.x, elementSize.y));
	mid.setTextureRect(IntRect(midNum * elementSize.x, 0, elementSize.x, elementSize.y));
	last.setTextureRect(IntRect(lastNum * elementSize.x, 0, elementSize.x, elementSize.y));
}

ShopLocation::FeelingTypes ShopLocation::checkAnswer(const QuestionType& type)
{
	const Personality interlocutorPersonality = chars.at(interlocutor->getType());
	const Personality::Relation relation = interlocutorPersonality.relations.at(currentCharacter->getType());
	int min = relation.min;
	int max = relation.max;
	int modifier = 0;
	switch(type)
	{
	case Hiring:
		modifier = 0;
		break;
	case Money:
		modifier = 4;
		break;
	default:
		modifier = 0;
	}
	const int result = min + rand() % max - modifier;

	FeelingTypes atype = Undefined;
	switch(result)
	{
	case 0:
		atype = Hard_not;
		break;
	case 1:
	case 2:
		atype = Not;
		break;
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
		atype = May_be;
		break;
	case 8:
	case 9:
		atype = Yes;
		break;
	case 10:
		atype = Absolutly;
		break;
	}
	return atype;
}

void ShopLocation::resolveAction(QuestionType action)
{
	switch(action)
	{
	case Money:
		//			float score = Char.money * bounty ;
		//		player.add score(score);
		break;
	case Mission:
		//				StartMission
		break;
	case Hiring:
		//		Adda Char
		break;
	}
}

void ShopLocation::fillPersonalities()
{
	//DOUBLE_DAMAGE_CHARACTER
	Personality pers;

	Personality::Relation relation;
	relation.min = 6;
	relation.max = 10;
	pers.relations.insert(pair<CHARACTERS, Personality::Relation>(SUPLY_CHARACTER, relation));
	relation.min = 0;
	relation.max = 6;
	pers.relations.insert(pair<CHARACTERS, Personality::Relation>(REPAIR_CHARACTER, relation));

	relation.min = 0;
	relation.max = 10;
	pers.relations.insert(pair<CHARACTERS, Personality::Relation>(DOUBLE_DAMAGE_CHARACTER, relation));

	vector<string> questions;
	//Hiring
	questions.push_back("Go to crew?");
	questions.push_back("Do u wanna join?");
	questions.push_back("Do u wanna join222?");
	pers.questions.insert(pair<QuestionType, vector<string>>(Hiring, questions));
	questions.clear();

	//Money
	questions.push_back("have a money?");
	questions.push_back("mone2222");
	questions.push_back("money333");
	pers.questions.insert(pair<QuestionType, vector<string>>(Money, questions));
	questions.clear();

	//Mission
	questions.push_back("have mission?");
	questions.push_back("Do u have any mission for me?");
	questions.push_back("Get mission");
	pers.questions.insert(pair<QuestionType, vector<string>>(Mission, questions));
	questions.clear();

	//HiringForMoney
	questions.push_back("HiringForMoney?");
	questions.push_back("HiringForMoney2?");
	questions.push_back("HiringForMoney3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringForMoney, questions));
	questions.clear();

	//HiringForMoneyNo
	questions.push_back("HiringForMoneyNo1?");
	questions.push_back("HiringForMoneyNo2?");
	questions.push_back("HiringForMoneyNo3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringForMoneyNo, questions));
	questions.clear();

	//HiringForMoneyYes
	questions.push_back("HiringForMoneyYes1?");
	questions.push_back("HiringForMoneyYes2?");
	questions.push_back("HiringForMoneyYes3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringForMoneyYes, questions));
	questions.clear();

	//HiringKick
	questions.push_back("HiringKick1?");
	questions.push_back("HiringKick2?");
	questions.push_back("HiringKick3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringKick, questions));
	questions.clear();

	//HiringKickYes
	questions.push_back("HiringKickYes1?");
	questions.push_back("HiringKickYes2?");
	questions.push_back("HiringKickYes3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringKickYes, questions));
	questions.clear();

	//HiringKickNo
	questions.push_back("HiringKickNo1?");
	questions.push_back("HiringKickNo2?");
	questions.push_back("HiringKickNo3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringKickNo, questions));
	questions.clear();

	//HiringSay
	questions.push_back("HiringSay1?");
	questions.push_back("HiringSay2?");
	questions.push_back("HiringSay3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringSay, questions));
	questions.clear();

	//HiringSayYes
	questions.push_back("HiringSayYes1?");
	questions.push_back("HiringSayYes2?");
	questions.push_back("HiringSayYes3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringSayYes, questions));
	questions.clear();

	//HiringSayNo
	questions.push_back("HiringSayNo1?");
	questions.push_back("HiringSayNo2?");
	questions.push_back("HiringSayNo3");
	pers.questions.insert(pair<QuestionType, vector<string>>(HiringSayNo, questions));
	questions.clear();

	//MissionForMoney
	questions.push_back("MissionForMoney1?");
	questions.push_back("MissionForMoney2?");
	questions.push_back("MissionForMoney23");
	pers.questions.insert(pair<QuestionType, vector<string>>(MissionForMoney, questions));
	questions.clear();

	//Say
	questions.push_back("asdf");
	questions.push_back("dkfjhksldhfg");
	questions.push_back("xdxdxd");
	pers.questions.insert(pair<QuestionType, vector<string>>(Say, questions));
	questions.clear();

	map<FeelingTypes, Personality::Answer> answerMap;
	Personality::Answer answer;
	//Hiring answers
	answer.text.push_back("Hard_not1");
	answer.text.push_back("Hard_not2");
	answer.text.push_back("Hard_not3");
	answer.action = Hiring;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Hard_not, answer));
	answer.text.clear();

	answer.text.push_back("Not1");
	answer.text.push_back("Not2");
	answer.text.push_back("Not3");
	answer.action = Hiring;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Not, answer));
	answer.text.clear();

	answer.text.push_back("May_be1");
	answer.text.push_back("May_be2");
	answer.text.push_back("May_be3");
	answer.action = Hiring;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(May_be, answer));
	answer.text.clear();

	answer.text.push_back("Yes1");
	answer.text.push_back("Ye2s");
	answer.text.push_back("Yes3");
	answer.action = Hiring;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Yes, answer));
	answer.text.clear();

	answer.text.push_back("Absolutly1");
	answer.text.push_back("Absolutly2");
	answer.text.push_back("Absolutly3");
	answer.action = Hiring;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Absolutly, answer));
	answer.text.clear();


	//HiringForMoney answers
	answer.text.push_back("Hard_not1");
	answer.text.push_back("Hard_not2");
	answer.text.push_back("Hard_not3");
	answer.action = HiringForMoney;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Hard_not, answer));
	answer.text.clear();

	answer.text.push_back("Not1");
	answer.text.push_back("Not2");
	answer.text.push_back("Not3");
	answer.action = HiringForMoney;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Not, answer));
	answer.text.clear();

	answer.text.push_back("May_be1");
	answer.text.push_back("May_be2");
	answer.text.push_back("May_be3");
	answer.action = HiringForMoney;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(May_be, answer));
	answer.text.clear();

	answer.text.push_back("Yes1");
	answer.text.push_back("Ye2s");
	answer.text.push_back("Yes3");
	answer.action = HiringForMoney;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Yes, answer));
	answer.text.clear();

	answer.text.push_back("Absolutly1");
	answer.text.push_back("Absolutly2");
	answer.text.push_back("Absolutly3");
	answer.action = HiringForMoney;
	answerMap.insert(pair<FeelingTypes, Personality::Answer>(Absolutly, answer));
	answer.text.clear();


	pers.answers.insert(pair<QuestionType, map<FeelingTypes, Personality::Answer>>(Hiring, answerMap));


//	answerMap.clear();

//	pers.answers.insert(pair<QuestionType, map<int, Personality::Answer>>(Money, answerMap));
//	answerMap.clear();
//	questions.clear();

//	pers.answers.insert(pair<QuestionType, map<int, Personality::Answer>>(Mission, answerMap));
//	answerMap.clear();
//	questions.clear();

//	pers.answers.insert(pair<QuestionType, map<int, Personality::Answer>>(Say, answerMap));
//	answerMap.clear();
//	questions.clear();

	chars.insert(pair<CHARACTERS, Personality>(DOUBLE_DAMAGE_CHARACTER, pers));

	//REPAIR_CHARACTER

	chars.insert(pair<CHARACTERS, Personality>(REPAIR_CHARACTER, pers));
}

ShopLocation::Dialogs ShopLocation::getDialog(const vector<string> &questions, ShopLocation::QuestionType type)
{
	srand(time(nullptr));

	Dialogs dlg;
	const int n = rand() % questions.size();
	const string text = questions.at(n);
	dlg.text.setString(text);
	dlg.text.setFont(GlobalVariables::Instance().font());
	dlg.text.setFillColor(Color::Green);
	dlg.type = type;
	return dlg;
}

void ShopLocation::updateCurrentChoose()
{
	const Dialogs dlg = dialogs.at(currentAnswer);
	chooseAnswer.sprite.setPosition(chooseAnswer.sprite.getPosition().x, dlg.text.getPosition().y);
}

void ShopLocation::fillDialogs(const vector<QuestionType> &questions)
{
	const float offset = GLOBAL_OFFSET * Settings::Instance().getScaleFactor().y;
	float xPos = dialogPanel.sprite.getPosition().x + offset;
	float yPos = dialogPanel.sprite.getPosition().y + offset;

	currentCharacter->getIcon().setPosition(xPos, yPos);
	xPos += offset + currentCharacter->getIcon().getGlobalBounds().width;
	chooseAnswer.sprite.setPosition(xPos, 0);
	xPos += offset + chooseAnswer.sprite.getGlobalBounds().width;

	const CHARACTERS character = DOUBLE_DAMAGE_CHARACTER;//currentCharacter->getType();
	const Personality personality = chars.at(character);

	dialogs.clear();


	for(QuestionType type : questions)
	{
		Dialogs dlg = getDialog(personality.questions.at(type), type);
		dlg.text.setPosition(xPos, yPos);
		yPos += dlg.text.getGlobalBounds().height + offset;
		dialogs.push_back(dlg);

	}

	currentAnswer = 0;
}

vector<ShopLocation::QuestionType> ShopLocation::getQuestions(ShopLocation::QuestionType question)
{
	vector<ShopLocation::QuestionType> result;
	switch (question)
	{
	case Default_question:
	{
		result.push_back(Hiring);
		result.push_back(Money);
		result.push_back(Mission);
		result.push_back(Say);
	}
		break;
	case HiringForMoney:
	{
		result.push_back(HiringForMoneyNo);
		result.push_back(HiringForMoneyYes);
	}
		break;
	case HiringKick:
	{
		result.push_back(HiringKickYes);
		result.push_back(HiringKickNo);
	}
		break;
	case HiringSay:
	{
		result.push_back(HiringSayNo);
		result.push_back(HiringSayYes);
	}
		break;
	case MissionForMoney:
	{
		result.push_back(MissionForMoneyNo);
		result.push_back(MissionForMoneyYes);
	}
		break;
	default:
		break;
	}
	return result;
}

ShopLocation::QuestionType ShopLocation::updateQuestionType(ShopLocation::QuestionType type) const
{
	QuestionType result = Default_question;
	switch (type)
	{
	case Hiring:
	{
		const int n = rand() % 2;
		switch (n)
		{
		case 0:
			result = HiringForMoney;
			break;
		case 1:
			result = HiringKick;
			break;
		case 2:
			result = HiringSay;
			break;
		default:
			break;
		}
	}
		break;
	default:
		break;
	}
	return result;
}
