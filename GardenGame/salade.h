#pragma once
#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

#define SALADEY 4
#define SALADEX 10
#define XSALADE 260

typedef struct
{
	int x;
	int y;
	bool Visible;    // 0 = non 1 = oui
	ALLEGRO_BITMAP* Image;
} SALADE;

void RecupSalade(SALADE Tab[][SALADEY]);
void AfficheSalade(SALADE pSalade[][SALADEY]);
void PousseSalade(SALADE pSalade[][SALADEY]);
