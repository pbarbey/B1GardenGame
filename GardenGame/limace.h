// limace .h

#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include "Personnage.h"
#include "salade.h"

#define TETE_LIMACE_DE_DROITE 15
#define TETE_LIMACE_DE_GAUCHE 90
#define TAILLE_COLLISION_SALADE 25
#define POS_LIMACE_Y 40
#define POS_LIMACE_Y_FERNANAD 50
#define POS_FERNAND_Y 35

const int NBIMAGE = 4;
const int IMTX = 100;
const int IMTY = 50;

const int SCRX = 800;
const int SCRY = 600;

typedef struct
{
	//deplacement
	float x, y; //position
	float dx, dy; //déplacement

	//image
	int tx, ty; //taille

	//animation
	int imcourante; //image courante
	int nbimage; //nombre d'image
	int tour; //compte tour
	int nbtour; //nombre de tour
	int dir; //direction de l'animation
}t_sprite;

ALLEGRO_BITMAP* recup_sprite(ALLEGRO_BITMAP* scr, int tx, int ty, int startx, int starty, int colonne, int i);

t_sprite* init_sprite(int PosDepart, float Vitesse);
void cntl_anim(t_sprite* s);
void avance(t_sprite* s);

bool CollisionLimaceSaladeGauche(t_sprite* pLimace, SALADE pSalade[][SALADEY]);
bool CollisionLimaceSaladeDroite(t_sprite* pLimace, SALADE pSalade[][SALADEY]);
bool CollisionFernandLimaceDroite(t_sprite* pLimace, image_personnage* pFernand, bool* key);
bool CollisionFernandLimaceGauche(t_sprite* pLimace, image_personnage* pFernand, bool* key);


// fin du limace .h
