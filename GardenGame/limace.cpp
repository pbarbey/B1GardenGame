// limace .c

#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include "limace.h"
#include "salade.h"
#include "personnage.h"
#include <time.h>


//on vient de roite
bool CollisionLimaceSaladeDroite(t_sprite* pLimace, SALADE pSalade[][SALADEY])
{
	bool ret = 0;
	int i, j;

	for (i = 0; i < SALADEX; i++)
		for (j = 0; j < SALADEY; j++)
			if (pSalade[i][j].Visible == 1)
				if (pLimace->x + TETE_LIMACE_DE_DROITE >= pSalade[i][j].x && pLimace->x + TETE_LIMACE_DE_DROITE < pSalade[i][j].x + TAILLE_COLLISION_SALADE
					&& pLimace->y + POS_LIMACE_Y >= pSalade[i][j].y && pLimace->y + POS_LIMACE_Y < pSalade[i][j].y + TAILLE_COLLISION_SALADE)
				{
					ret = 1;  // il y a collision
					pSalade[i][j].Visible = 0;
					pLimace->dx = -0.2;
				}

	return ret;
}

//on vient de gauche
bool CollisionLimaceSaladeGauche(t_sprite* pLimace, SALADE pSalade[][SALADEY])
{
	bool ret = 0;
	int i, j;

	for (i = 0; i < SALADEX; i++)
		for (j = 0; j < SALADEY; j++)
			if (pSalade[i][j].Visible == 1)
				if (pLimace->x + TETE_LIMACE_DE_GAUCHE >= pSalade[i][j].x && pLimace->x + TETE_LIMACE_DE_GAUCHE < pSalade[i][j].x + TAILLE_COLLISION_SALADE
					&& pLimace->y + POS_LIMACE_Y >= pSalade[i][j].y && pLimace->y + POS_LIMACE_Y < pSalade[i][j].y + TAILLE_COLLISION_SALADE)
				{
					ret = 1;  // il y a collision
					pSalade[i][j].Visible = 0;
					pLimace->dx = 0.2;
				}

	return ret;
}


bool CollisionFernandLimaceDroite(t_sprite* pLimace, image_personnage* pFernand, bool* key)
{
	bool ret = 0;

	if (pFernand->x >= pLimace->x && pFernand->x <= pLimace->x + POS_LIMACE_Y_FERNANAD && pFernand->y + POS_FERNAND_Y >= pLimace->y
		&& pFernand->y + POS_FERNAND_Y <= pLimace->y + POS_LIMACE_Y_FERNANAD)
	{
		pLimace->x = SCRX;
		pLimace->dx = -1.0;
		pLimace->y = 200 + rand() % 150;
		ret = 1;
	}

	return ret;
}

bool CollisionFernandLimaceGauche(t_sprite* pLimace, image_personnage* pFernand, bool* key)
{
	bool ret = 0;

	if (pFernand->x >= pLimace->x && pFernand->x <= pLimace->x + POS_LIMACE_Y_FERNANAD && pFernand->y + POS_FERNAND_Y >= pLimace->y
		&& pFernand->y + POS_FERNAND_Y <= pLimace->y + POS_LIMACE_Y_FERNANAD)
	{
		pLimace->x = -pLimace->tx;
		pLimace->dx = 1.0;
		pLimace->y = 200 + rand() % 150;
		ret = 1;
	}

	return ret;
}



t_sprite* init_sprite(int PosDepart, float Vitesse)
{
	t_sprite* s = (t_sprite*)malloc(sizeof(t_sprite));

	s->x = PosDepart;
	s->y = rand() % 400;
	s->dx = Vitesse;
	s->dy = 0.0;
	s->tx = IMTX;
	s->ty = IMTY;
	s->imcourante = rand() % 3;
	s->nbimage = NBIMAGE;
	s->tour = 0;
	s->nbtour = 10;
	s->dir = 1;

	return s;
}

ALLEGRO_BITMAP* recup_sprite(       //récupérer sur un fichier la séquence d'animation

	ALLEGRO_BITMAP* scr, //bitmap df'origine,
	int tx, int ty, //taille élément
	int startx, int starty, //à partir de
	int colonne, //nombres de colonne
	int i)//ième élément
{

	ALLEGRO_BITMAP* sprite = NULL;
	int x, y;

	sprite = al_create_bitmap(tx, ty);
	if (sprite != NULL)
	{
		//colonne doit être > 0
		x = startx + (i % 1) * tx;
		y = starty + (i % 1) * ty;

		al_set_target_bitmap(sprite);
		al_draw_bitmap_region(scr, x, y, tx, ty, 0, 0, 0);
	}
	return sprite;
}

void cntl_anim(t_sprite* s) //controler l'animation des images
{
	//nbtour doit être > 0
	s->tour = (s->tour + 1) % s->nbtour;
	if (s->tour == 0)
	{

		s->imcourante += s->dir;
		//rester entre 0 et (nbimage-1) compris
		s->imcourante = (s->imcourante + s->nbimage) % s->nbimage;
	}

}

void avance(t_sprite* s)			//controle du déplacement, si un bord est touché le personnage repart
{

	s->x += s->dx;
	s->y += s->dy;

	//sortie à gauche entrée à droite
	if (s->x + s->tx < 0)
	{
		s->x = SCRX;
		s->dx = -1.0;
		s->y = 200 + rand() % 150;
	}
	// sortie à droite entrée à gauche
	else if (s->x > SCRX)
	{
		s->x = -s->tx;
		s->dx = +1.0;
	}
	//quand la limace sort des salades elle réaccélére
	if (s->dx < 0)//on vient de droite 
	{
		if (s->x < XSALADE)
			s->dx = -1.0;
	}
	else //on vient de gauche
		if (s->x > 425)
			s->dx = 1.0;
}

//fin de limace.cpp

