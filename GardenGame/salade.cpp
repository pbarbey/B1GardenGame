#pragma
#include "stdafx.h"
#include "salade.h"


void RecupSalade(SALADE pSalade[][4])
{
	// récupération du fichier conteneur
	ALLEGRO_BITMAP* conteneur;
	ALLEGRO_BITMAP* pS;
	int i, j;


	for (i = 0; i < SALADEX; i++)
		for (j = 0; j < SALADEY; j++)
		{
			pSalade[i][j].x = XSALADE + (i * 25) + 25;
			pSalade[i][j].y = 225 + (j * 25) + 25;
			pSalade[i][j].Image = al_load_bitmap("anim/1Salade.bmp");
			al_convert_mask_to_alpha(pSalade[i][j].Image, al_get_pixel(pSalade[i][j].Image, 0, 0));
			pSalade[i][j].Visible = 1;

		}
}

void  AfficheSalade(SALADE pSalade[][4])
{
	int i, j;

	for (i = 0; i < SALADEX; i++)
		for (j = 0; j < SALADEY; j++)
			if (pSalade[i][j].Visible == 1)
				al_draw_bitmap(pSalade[i][j].Image, pSalade[i][j].x, pSalade[i][j].y, 0);

}


void PousseSalade(SALADE pSalade[][SALADEY])
{
	int i, j, memo;
	memo = 0;
	for (i = 0; i < SALADEX; i++)
		for (j = 0; j < SALADEY; j++)
		{
			if ((pSalade[i][j].Visible == 0) && (memo == 0))
			{
				pSalade[i][j].Visible = 1;
				memo = 1;
			}
		}

}

