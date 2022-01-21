/*
 * File:   collision.cpp
 * Author: Nick (original version), ahnonay (SFML2 compatibility)
 */
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include "collisions.h"

#include "arcsprite.h"
#include "arcrect.h"

namespace Collision
{
        class BitmaskManager
        {
        public:
                ~BitmaskManager() {
                        std::map<const sf::Texture*, sf::Uint8*>::const_iterator end = Bitmasks.end();
                        for (std::map<const sf::Texture*, sf::Uint8*>::const_iterator iter = Bitmasks.begin(); iter!=end; iter++)
                                delete [] iter->second;
                }

                sf::Uint8 GetPixel (const sf::Uint8* mask, const sf::Texture* tex, unsigned int x, unsigned int y) {
                        if (x>tex->getSize().x||y>tex->getSize().y)
                                return 0;

                        return mask[x+y*tex->getSize().x];
                }

				sf::Uint8* GetMask (const sf::Texture* tex) {
                        sf::Uint8* mask;
                        std::map<const sf::Texture*, sf::Uint8*>::iterator pair = Bitmasks.find(tex);
                        if (pair==Bitmasks.end())
                        {
								const sf::Image img = tex->copyToImage();
                                mask = CreateMask (tex, img);
                        }
                        else
                                mask = pair->second;

                        return mask;
                }

                sf::Uint8* CreateMask (const sf::Texture* tex, const sf::Image& img) {
                        sf::Uint8* mask = new sf::Uint8[tex->getSize().y*tex->getSize().x];

                        for (unsigned int y = 0; y<tex->getSize().y; y++)
                        {
                                for (unsigned int x = 0; x<tex->getSize().x; x++)
                                        mask[x+y*tex->getSize().x] = img.getPixel(x,y).a;
                        }

                        Bitmasks.insert(std::pair<const sf::Texture*, sf::Uint8*>(tex,mask));

                        return mask;
                }
        private:
                std::map<const sf::Texture*, sf::Uint8*> Bitmasks;
        };

        BitmaskManager Bitmasks;

        bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, sf::Uint8 AlphaLimit) {
                sf::FloatRect Intersection;
                if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds(), Intersection)) {
                        sf::IntRect O1SubRect = Object1.getTextureRect();
                        sf::IntRect O2SubRect = Object2.getTextureRect();

						const sf::Texture *texture1 = Object1.getTexture();
						const sf::Texture *texture2 = Object2.getTexture();

						sf::Uint8* mask1 = Bitmasks.GetMask(texture1);
						sf::Uint8* mask2 = Bitmasks.GetMask(texture2);

						// Loop through our pixels
                        for (int i = Intersection.left; i < Intersection.left+Intersection.width; i++) {
                                for (int j = Intersection.top; j < Intersection.top+Intersection.height; j++) {

                                        sf::Vector2f o1v = Object1.getInverseTransform().transformPoint(i, j);
                                        sf::Vector2f o2v = Object2.getInverseTransform().transformPoint(i, j);

                                        // Make sure pixels fall within the sprite's subrect
                                        if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
                                                o1v.x < O1SubRect.width && o1v.y < O1SubRect.height &&
                                                o2v.x < O2SubRect.width && o2v.y < O2SubRect.height) {

                                                        if (Bitmasks.GetPixel(mask1, Object1.getTexture(), (int)(o1v.x)+O1SubRect.left, (int)(o1v.y)+O1SubRect.top) > AlphaLimit &&
                                                                Bitmasks.GetPixel(mask2, Object2.getTexture(), (int)(o2v.x)+O2SubRect.left, (int)(o2v.y)+O2SubRect.top) > AlphaLimit)
                                                                return true;

                                        }
                                }
                        }
                }
                return false;
        }

        bool CreateTextureAndBitmask(sf::Texture &LoadInto, const std::string& Filename)
        {
                sf::Image img;
                if (!img.loadFromFile(Filename))
                        return false;
                if (!LoadInto.loadFromImage(img))
                        return false;

                Bitmasks.CreateMask(&LoadInto, img);
                return true;
        }

        sf::Vector2f GetSpriteCenter (const sf::Sprite& Object)
        {
                sf::FloatRect AABB = Object.getGlobalBounds();
                return sf::Vector2f (AABB.left+AABB.width/2.f, AABB.top+AABB.height/2.f);
        }

        sf::Vector2f GetSpriteSize (const sf::Sprite& Object)
        {
                sf::IntRect OriginalSize = Object.getTextureRect();
                sf::Vector2f Scale = Object.getScale();
                return sf::Vector2f (OriginalSize.width*Scale.x, OriginalSize.height*Scale.y);
        }

        bool CircleTest(const sf::Sprite& Object1, const sf::Sprite& Object2) {
                sf::Vector2f Obj1Size = GetSpriteSize(Object1);
                sf::Vector2f Obj2Size = GetSpriteSize(Object2);
                float Radius1 = (Obj1Size.x + Obj1Size.y) / 4;
                float Radius2 = (Obj2Size.x + Obj2Size.y) / 4;

                sf::Vector2f Distance = GetSpriteCenter(Object1)-GetSpriteCenter(Object2);

                return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
        }

        class OrientedBoundingBox // Used in the BoundingBoxTest
        {
        public:
                OrientedBoundingBox (const sf::Sprite& Object) // Calculate the four points of the OBB from a transformed (scaled, rotated...) sprite
                {
                        sf::Transform trans = Object.getTransform();
                        sf::IntRect local = Object.getTextureRect();
                        Points[0] = trans.transformPoint(0.f, 0.f);
                        Points[1] = trans.transformPoint(local.width, 0.f);
                        Points[2] = trans.transformPoint(local.width, local.height);
                        Points[3] = trans.transformPoint(0.f, local.height);
                }

                sf::Vector2f Points[4];

                void ProjectOntoAxis (const sf::Vector2f& Axis, float& Min, float& Max) // Project all four points of the OBB onto the given axis and return the dotproducts of the two outermost points
                {
                        Min = (Points[0].x*Axis.x+Points[0].y*Axis.y);
                        Max = Min;
                        for (int j = 1; j<4; j++)
                        {
                                float Projection = (Points[j].x*Axis.x+Points[j].y*Axis.y);

                                if (Projection<Min)
                                        Min=Projection;
                                if (Projection>Max)
                                        Max=Projection;
                        }
                }
        };

        bool BoundingBoxTest(const sf::Sprite& Object1, const sf::Sprite& Object2) {
                OrientedBoundingBox OBB1 (Object1);
                OrientedBoundingBox OBB2 (Object2);

                // Create the four distinct axes that are perpendicular to the edges of the two rectangles
                sf::Vector2f Axes[4] = {
                        sf::Vector2f (OBB1.Points[1].x-OBB1.Points[0].x,
                        OBB1.Points[1].y-OBB1.Points[0].y),
                        sf::Vector2f (OBB1.Points[1].x-OBB1.Points[2].x,
                        OBB1.Points[1].y-OBB1.Points[2].y),
                        sf::Vector2f (OBB2.Points[0].x-OBB2.Points[3].x,
                        OBB2.Points[0].y-OBB2.Points[3].y),
                        sf::Vector2f (OBB2.Points[0].x-OBB2.Points[1].x,
                        OBB2.Points[0].y-OBB2.Points[1].y)
                };

                for (int i = 0; i<4; i++) // For each axis...
                {
                        float MinOBB1, MaxOBB1, MinOBB2, MaxOBB2;

                        // ... project the points of both OBBs onto the axis ...
                        OBB1.ProjectOntoAxis(Axes[i], MinOBB1, MaxOBB1);
                        OBB2.ProjectOntoAxis(Axes[i], MinOBB2, MaxOBB2);

                        // ... and check whether the outermost projected points of both OBBs overlap.
                        // If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
                        if (!((MinOBB2<=MaxOBB1)&&(MaxOBB2>=MinOBB1)))
                                return false;
                }
                return true;
		}
}

bool Intersection::isParallelogramContainsPoint(const sf::Vector2f& point,
												const std::vector<sf::Vector2f> &coords)
{
	if (coords.size() != 4)
		return false;

	const sf::Vector2f A = coords.at(0);
	const sf::Vector2f B = coords.at(1);
	const sf::Vector2f C = coords.at(2);
	const sf::Vector2f D = coords.at(3);

	const float P1 = (A.x - point.x) * (B.y - A.y) - (A.y - point.y) * (B.x - A.x);
	const float P2 = (B.x - point.x) * (C.y - B.y) - (B.y - point.y) * (C.x - B.x);
	const float P3 = (C.x - point.x) * (D.y - C.y) - (C.y - point.y) * (D.x - C.x);
	const float P4 = (D.x - point.x) * (A.y - D.y) - (D.y - point.y) * (A.x - D.x);

	if ((P1 < 0 && P2 < 0 && P3 < 0 && P4 < 0) ||
			(P1 > 0 && P2 > 0 && P3 > 0 && P4 > 0))
		return true;
	return false;
}

bool Intersection::contains(const sf::RectangleShape& rect,
							const sf::Vector2f& pos,
							const sf::Transform &transform)
{
	const float x = rect.getGlobalBounds().left;
	const float y = rect.getGlobalBounds().top;
	const float w = x + rect.getGlobalBounds().width;
	const float h = y + rect.getGlobalBounds().height;

	const sf::Vector2f p1 = transform.transformPoint(x, y);
	const sf::Vector2f p2 = transform.transformPoint(x, h);
	const sf::Vector2f p3 = transform.transformPoint(w, h);
	const sf::Vector2f p4 = transform.transformPoint(w, y);

	const std::vector<sf::Vector2f> points = {p1, p2, p3, p4};

	return isParallelogramContainsPoint(pos, points);
}

bool Intersection::intersects(const sf::RectangleShape &objectRect, const sf::FloatRect &rect,
							  const sf::Transform &transform)
{
	std::vector<sf::Vector2f> positions = {
		sf::Vector2f(rect.left, rect.top),
		sf::Vector2f(rect.left + rect.width, rect.top),
		sf::Vector2f(rect.left + rect.width, rect.top + rect.height),
		sf::Vector2f(rect.left, rect.top + rect.height)
	};
	for(const sf::Vector2f& pos : positions) {
		if (contains(objectRect, pos, transform))
			return true;
	}
	return false;
}

sf::Vector2f Intersection::getTranslatedPoint(ArcObject *object, const sf::Vector2f &point)
{
	return object->m_transform.transformPoint(point);
}

bool Intersection::contains(ArcObject *object, const sf::Vector2f &pos)
{
	switch (object->type())
	{
	case ArcEngine::SPRITE:
	{
		ArcSprite *sprite = static_cast<ArcSprite*>(object);
		return contains(sprite->sprite, pos, sprite->m_transform);
	}
	case ArcEngine::RECT:
	{
		ArcRect *rectObject = static_cast<ArcRect*>(object);
		return contains(rectObject->rect, pos, rectObject->m_transform);
	}
	default:
#ifdef ARC_DEBUG
		return contains(object->debugRect, pos, object->m_transform);
#else
		break;
#endif
	}
	return false;
}

bool Intersection::intersects(ArcObject *object, const sf::FloatRect &rect)
{
	switch (object->type())
	{
	case ArcEngine::SPRITE:
	{
		ArcSprite *sprite = static_cast<ArcSprite*>(object);
		return intersects(sprite->sprite, rect);
	}
	case ArcEngine::RECT:
	{
		ArcRect *rectObject = static_cast<ArcRect*>(object);
		return intersects(rectObject->rect, rect);
	}
	default:
#ifdef ARC_DEBUG
		return intersects(object->debugRect, rect, object->m_transform);
#else
		break;
#endif
	}
	return false;
}
