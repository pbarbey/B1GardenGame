#pragma once
#include "stdafx.h"
#include "salade.h"

enum KEYS { UP, RIGHT, DOWN, LEFT, KEY_MAX };

typedef struct image_personne
{
	ALLEGRO_BITMAP* image;
	int x;
	int y;

}image_personnage;

bool CollisionFernandSalade(image_personnage*, SALADE pSalade[][SALADEY], bool*);
