#include "level.h"
#include "Game/gameobject.h"
#include "settings.h"
#include "Game/SpaceShip/spaceship.h"
#include "SpaceShip/Weapons/abstractweapon.h"
#include "effects.h"
#include "gamepanel.h"
#include "objectsfactory.h"
#include "cpu.h"
#include "Game/Audio/soundcontroller.h"
#include "globalvariables.h"
#include "gamecontroller.h"
#include "Artifacts/bonus.h"
#include "Collisions/collisions.h"
#include "savedgame.h"
#include "ResourcesManager/resourcesmanager.h"

constexpr static int TEXTURE_HEIGHT = 2048;
constexpr static int MAX_SHAKE_OFFSET = 5;
constexpr static int SHAKE_DURATION = 500;

Level::Level() :
	m_finished(false)
  ,m_isFailed(false)
  ,difficulty(1.f)
{
	boss.spaceShip = nullptr;
	shake.isActive = false;
	panel = new GamePanel(this);

	if (!levelShader.loadFromFile("images/smoke.frag", Shader::Fragment))
	{
		err() << "Failed to load shader, exiting..." << std::endl;
		return;
	}
//	levelShader.setUniform("resolution", Settings::Instance().getResolution());
	levelShader.setParameter("resolution", Vector2f(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y));
	smokeTimer.restart();
	smokeEnabled = false;
	levelRect.setSize(Vector2f(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y));


	const float k = static_cast<float>(Settings::Instance().getInscribedResolution().y)/Settings::Instance().getResolution().y;
	resolutionOffsetX = Settings::Instance().getResolution().x - k * Settings::Instance().getResolution().x;
	resolutionOffsetX /= 2;
}

Level::~Level()
{
	delete panel;
}

void Level::draw(RenderWindow *const window)
{
	int offset = 0;
	if (shake.isActive)
	{
		offset = MAX_SHAKE_OFFSET - rand() % MAX_SHAKE_OFFSET * 2;
		view.setCenter(view.getCenter().x, m_viewCenter - offset);
		panel->setTop(m_topBorder - offset);
	}

	window->setView(view);

	for(const Sprite& sprite : sprites)
		window->draw(sprite);

	//	levelShader.setUniform("time", smokeTimer.getElapsedTime().asSeconds());
	levelShader.setParameter("time", smokeTimer.getElapsedTime().asSeconds());
//	levelShader.setParameter("topLeft", Vector2f(topBorder(), leftBorder()));
	if (smokeEnabled)
		window->draw(levelRect, &levelShader);

	for(Bonus *bonus : bonuses)
		bonus->draw(window);

	for(LevelEnemy& levelEnemy : enemies)
	{
		levelEnemy.spaceShip->draw(window);
		levelEnemy.drawLifeBar(window);
	}
	if (boss.spaceShip != nullptr)
	{
		boss.spaceShip->draw(window);
		boss.drawLifeBar(window);
	}

	for(SpaceShip *spaceShip : GameController::Instance().players())
		spaceShip->draw(window);

	Effects::Instance().draw(window);

	panel->draw(window);
}

void Level::update()
{
	if (shake.timer.getElapsedTime().asMilliseconds() > SHAKE_DURATION)
		shakeOff();

	checkDeathZone();

	if (timer.check(GlobalVariables::FRAME_TIME))
	{
		calculateCollisions();
		m_topBorder -= m_speed;
		m_bottomBorder -= m_speed;
		m_viewCenter -= m_speed;
		CPU::Instance().action(enemies, -m_speed);

		for(SpaceShip *spaceShip : GameController::Instance().players())
			spaceShip->move(0, -m_speed);

		checkRespawnLine(m_topBorder - Settings::Instance().getResolution().y * 0.1f);

		panel->setTop(m_topBorder);
		view.setCenter(view.getCenter().x, m_viewCenter);
		levelRect.setPosition(leftBorder(), topBorder());

		Effects::Instance().moveAnimnations(-m_speed);

		if (m_topBorder < levelEndPos)
		{
			if (boss.spaceShip == nullptr)
			{
				boss.spaceShip = ObjectsFactory::Instance().createSpaceShip(mission.boss.type, Vector2f(mission.boss.x + 200, 100 + levelEndPos));
				boss.setSize(boss.spaceShip->getSize());
				CPU::Instance().setBoss(boss.spaceShip);
			}
			addGraphics(0);
		}
	}

	for(Bonus *bonus : bonuses)
		bonus->update();

	CPU::Instance().update();

	for(LevelEnemy& levelEnemy : enemies)
	{
		levelEnemy.spaceShip->update();
		levelEnemy.spaceShip->updateWithLevel(this);
	}

	if (boss.spaceShip != nullptr)
		boss.spaceShip->update();

	for(SpaceShip *spaceShip : GameController::Instance().players())
	{
		spaceShip->updateWithLevel(this);
		spaceShip->update();
	}

	Effects::Instance().update();

	panel->update();
}

float Level::leftBorder() const
{
	return view.getCenter().x - view.getSize().x/2;
}

float Level::rightBorder() const
{
	return resolutionOffsetX + view.getCenter().x + view.getSize().x/2;
}

float Level::topBorder() const
{
	return m_topBorder + panel->height();
}

float Level::bottomBorder() const
{
	return m_bottomBorder + resolutionOffsetX/2;
}

Vector2f Level::getCenter() const
{
	return view.getCenter();
}

Vector2f Level::getStartingPos() const
{
	return m_startPos;
}

void Level::startMission(const unsigned int n)
{
	this->mission = Campaign::Instance().missions.at(n);
	SoundController::Instance().startBackgroundSound("sounds/map1.ogg");
	difficulty = 1.f + static_cast<float>(SavedGameLoader::Instance().getSavedGame().completedLevels.size()) / 10;
	fillLevel();
}

void Level::calculateCollisions()
{
	vector<SpaceShip*> currentEenmies;
	for(const LevelEnemy& levelEnemy : enemies)
		currentEenmies.push_back(levelEnemy.spaceShip);
	if (boss.spaceShip != nullptr)
		currentEenmies.push_back(boss.spaceShip);

	collideSpaceShips(GameController::Instance().players(), currentEenmies);//player collides

	currentEenmies.clear();
	for(const LevelEnemy& levelEnemy : enemies)
		currentEenmies.push_back(levelEnemy.spaceShip);
	if (boss.spaceShip != nullptr)
		currentEenmies.push_back(boss.spaceShip);

	collideSpaceShips(currentEenmies, GameController::Instance().players());	//enemies collides

	collideWithBonuses();
	collideSpaceShips();
	checkAlive();
}

void Level::collideSpaceShips(const vector<SpaceShip *>& attackers,
							  const vector<SpaceShip *> &defenders)
{
	for(SpaceShip *attacker : attackers)
	{
		vector<AbstractWeapon*> weapons;
		weapons.push_back(attacker->getDefaultWeapon());
		AbstractWeapon *currentWeapon = attacker->getCurrentWeapon();
		if (currentWeapon != nullptr)
			weapons.push_back(currentWeapon);

		const bool isEnemy = attacker->getPlayer() == -1;
		for(AbstractWeapon *weapon : weapons)
		{
			for(SpaceShip *defender : defenders)
			{
				const vector<Projectile*> projectiles = weapon->getProjectiles();
				for(Projectile *projectile : projectiles)
				{
					if (Collision::PixelPerfectTest(defender->getSprite(), projectile->getSprite()))
					{
						const bool critical = weapon->isCritical();

						float damage = weapon->damage();
						if (critical)
						{
							damage += damage * attacker->getStats().crit_damage;
							Effects::Instance().showAnimation(RESOURCES::CRITICAL_EFFECT,
															  defender->pos() + Vector2f(defender->gameRect().width/2,defender->gameRect().height/2),
															  Vector2i(32,32), 150, 4);
						}
						weapon->applyEffect(defender);

						//vampirism
						if (attacker->getCanHeal())
							attacker->heal(damage * 0.1f);

						if (isEnemy)
							defender->hit(damage * difficulty);
						else
							defender->hit(damage);

						//back damage
						if (defender->getBackDamage())
							attacker->hit(damage * 0.5f);

						weapon->actionProjectile(projectile);
						if (isEnemy)
						{
							shake.isActive = true;
							shake.timer.restart();
						}
						if (projectile == nullptr)
							continue;
					}
				}
			}
		}
	}
}

void Level::collideWithBonuses()
{
	for(SpaceShip *player : GameController::Instance().players())
	{
		for (auto it = bonuses.begin(); it != bonuses.end();)
		{
			Bonus *bonus = (*it);

			if(Collision::PixelPerfectTest(player->getSprite(), bonus->getSprite()))
			{
				const float bonusCount = bonus->getCount();
				levelBonuses[player] += bonusCount;

				SoundController::Instance().playOnce("sounds/turn end.ogg");
				delete bonus;
				it = bonuses.erase(it);
			}
			else
				++it;
		}
	}
}

void Level::checkRespawnLine(float y)
{
	auto it = mission.enemies.begin();
	for(;it != mission.enemies.end();)
	{
		if (it->pos.y > y)
		{
			addEnemy(*it);
			mission.enemies.erase(it);
		}
		else
			++it;
	}
}

void Level::addGraphics(const int n)
{
	Sprite sprite;
	Texture &texture = ResourcesManager::Instance().getMapTexture(mission.id, n);
	sprite.setTexture(texture);
	sprite.scale(Settings::Instance().getScaleFactor());
	levelEndPos -= texture.getSize().y * Settings::Instance().getScaleFactor().y;
	sprite.setPosition(0, levelEndPos);
	sprites.push_back(sprite);	
}

void Level::fillLevel()
{
	levelEndPos = TEXTURE_HEIGHT;

	for (int i = 0; i < mission.maps; ++i)
		addGraphics(i + 1);

	view.setSize(Vector2f(Settings::Instance().getResolution().x, Settings::Instance().getResolution().y));

	m_startPos.x = static_cast<float>(Settings::Instance().getResolution().x)/2;
	m_startPos.y = static_cast<float>(Settings::Instance().getResolution().y)/2;

	m_viewCenter = m_startPos.y;
	view.setCenter(m_startPos);

	m_topBorder = view.getCenter().y - view.getSize().y/2;
	m_bottomBorder = view.getCenter().y + view.getSize().y/2;

	m_speed = mission.speed;

	panel->setLeftBorder(leftBorder());

	SoundController::Instance().startBackgroundSound(mission.backgroundSound);
	for(SpaceShip *player : GameController::Instance().players())
		levelBonuses.insert(pair<SpaceShip*, float>(player, 0));
}

void Level::collideSpaceShips()
{
	for(SpaceShip *player : GameController::Instance().players())
	{
		for(const LevelEnemy& levelEnemy : enemies)
		{
			if (Collision::PixelPerfectTest(player->getSprite(), levelEnemy.spaceShip->getSprite()))
			{
				const float collisionDamagePlayer = levelEnemy.spaceShip->getDefaultData().health * 0.3f;
				player->hit(collisionDamagePlayer);

				const float collisionDamageEnemy = player->getDefaultData().health * 0.3f;
				levelEnemy.spaceShip->hit(collisionDamageEnemy);
			}
		}
		if (boss.spaceShip != nullptr)
		{			
			if (Collision::PixelPerfectTest(player->getSprite(), boss.spaceShip->getSprite()))
			{
				const float collisionDamagePlayer = boss.spaceShip->getDefaultData().health * 0.5f;
				player->hit(collisionDamagePlayer);

				const float collisionDamageBoss = player->getDefaultData().health * 0.3f;
				boss.spaceShip->hit(collisionDamageBoss);
			}
		}
	}
}

void Level::checkAlive()
{
	m_isFailed = !GameController::Instance().checkAlive();

	for (auto it = enemies.begin(); it != enemies.end();)
	{
		if (!it->spaceShip->isAlive())
		{
			const SPACESHIP_TYPES type = it->spaceShip->getType();

			//TODO Если будет вероятность выпадения бонуса в зависимости от сложности сделать тут
			const float bonusCount = Bonus::getBonusCount(type);
			Bonus* bonus = new Bonus(bonusCount, it->spaceShip->pos());
			bonuses.push_back(bonus);

//			Effects::Instance().showAnimation("images/Enemies/enemy.png", it->spaceShip->pos(), Vector2i(126, 126), 200, 6, 7);
			delete it->spaceShip;
			it = enemies.erase(it);
			SoundController::Instance().playOnce("sounds/unit destroyed.ogg");
		}
		else
			++it;
	}
	if (boss.spaceShip != nullptr && !boss.spaceShip->isAlive())
	{
		const float bonusCount = Bonus::getBonusCount(boss.spaceShip->getType());
		const float playerBonus = bonusCount / GameController::Instance().players().size();

		for(SpaceShip *player : GameController::Instance().players())
		{			
			const float bonus = levelBonuses.at(player) + playerBonus;
			player->addScore(bonus);
			player->addPoints(2 + difficulty);
		}
		m_finished = true;
	}
	if (m_isFailed)
		m_finished = true;

	if (m_finished)
		shakeOff();
}

void Level::checkDeathZone()
{
	const float BORDER_SIZE = Settings::Instance().getResolution().x;

	float leftBorderVal = leftBorder() - BORDER_SIZE;
	float rightBorderVal = rightBorder() + BORDER_SIZE;
	float topBorderVal = topBorder() - BORDER_SIZE;
	float bottomBorderVal = bottomBorder() + BORDER_SIZE;

	//enemies
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		const Vector2f pos = it->spaceShip->pos();
		if (pos.x > rightBorderVal||
				pos.x < leftBorderVal ||
				pos.y > bottomBorderVal ||
				pos.y < topBorderVal)
		{
			delete it->spaceShip;
			it = enemies.erase(it);
		}
		else
			++it;
	}

	//weapons
	for(const LevelEnemy& levelEnemy : enemies)
	{
		for(AbstractWeapon *weapon : levelEnemy.spaceShip->getWeapons())
			for(Projectile *projectile : weapon->getProjectiles())
			{
				const Vector2f pos = projectile->pos();

				if (pos.x > rightBorderVal||
						pos.x < leftBorderVal ||
						pos.y > bottomBorderVal ||
						pos.y < topBorderVal)
					weapon->removeProjectile(projectile);
			}
	}
	for(SpaceShip *spaceShip : GameController::Instance().players())
		for(AbstractWeapon *weapon : spaceShip->getWeapons())
			for(Projectile *projectile : weapon->getProjectiles())
			{
				const Vector2f pos = projectile->pos();

				if (pos.x > rightBorderVal||
						pos.x < leftBorderVal ||
						pos.y > bottomBorderVal ||
						pos.y < topBorderVal)
					weapon->removeProjectile(projectile);
			}

	if (boss.spaceShip != nullptr)
	for(AbstractWeapon *weapon : boss.spaceShip->getWeapons())
		for(Projectile *projectile : weapon->getProjectiles())
		{
			const Vector2f pos = projectile->pos();

			if (pos.x > rightBorderVal||
					pos.x < leftBorderVal ||
					pos.y > bottomBorderVal ||
					pos.y < topBorderVal)
				weapon->removeProjectile(projectile);
		}
}

map<SpaceShip *, float> Level::getLevelBonuses() const
{
	return levelBonuses;
}

SpaceShip *Level::addEnemy(const Mission::Enemy &mEnemy)
{
	SpaceShip *enemy = ObjectsFactory::Instance().createSpaceShip(mEnemy.spaceShipType, mEnemy.pos + Vector2f(0, 20 + panel->height()));
	LevelEnemy levelEnemy(enemy->getSize());
	levelEnemy.move = mEnemy.move;
	levelEnemy.spaceShip = enemy;
	GameState state;
	state.characteristics.health = state.characteristics.health * difficulty;
	state.characteristics.shield = state.characteristics.shield * difficulty;
	state.characteristics.reflection = state.characteristics.reflection * difficulty;
	state.characteristics.regen_rate = state.characteristics.regen_rate * difficulty;
	state.characteristics.crit_damage = state.characteristics.crit_damage * difficulty;

	state.spaceShip = mEnemy.spaceShipType;
	state.artifact = mEnemy.artifact;

	state.crew = mEnemy.characters;
	if (state.crew.empty())
		state.crew.push_back(CHARACTERS::ENEMY_CHARACTER);

	enemy->loadData(state);
	enemies.push_back(levelEnemy);
	return levelEnemy.spaceShip;
}

void Level::removeEnemy(SpaceShip *enemy)
{
	for (auto it = enemies.begin(); it != enemies.end();)
	{
		if (it->spaceShip == enemy)
		{
			delete it->spaceShip;
			enemies.erase(it);
			return;
		}
	}
}

void Level::shakeOff()
{
	shake.isActive = false;
}

bool Level::isFinished() const
{
	return m_finished;
}

vector<SpaceShip *> Level::getAllSpaceShips() const
{
	vector<SpaceShip *> spaceShips = GameController::Instance().players();
	for(const LevelEnemy& levelEnemy : enemies)
		spaceShips.push_back(levelEnemy.spaceShip);
	if (boss.spaceShip != nullptr)
		spaceShips.push_back(boss.spaceShip);
	return spaceShips;
}

bool Level::isFailed() const
{
	return m_isFailed;
}
