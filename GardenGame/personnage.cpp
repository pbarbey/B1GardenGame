#include "stdafx.h"
#include "Personnage.h"

bool CollisionFernandSalade(image_personnage* pFernand, SALADE pSalade[][SALADEY], bool* key)
{
	bool ret = 0;
	int i, j;


	for (i = 0; i < SALADEX; i++)
		for (j = 0; j < SALADEY; j++)
			if (pSalade[i][j].Visible == 1)
			{
				if (key[UP] == true)// on monte
				{
					if ((pFernand->x >= pSalade[i][j].x) && (pFernand->x <= pSalade[i][j].x + 25) && (pFernand->y + 35 - 1 > pSalade[i][j].y) && (pFernand->y + 35 - 1 < pSalade[i][j].y + 25))
						ret = 1;
				}
				else
				{
					if (key[DOWN] == true)// on descend
					{
						if ((pFernand->x >= pSalade[i][j].x) && (pFernand->x <= pSalade[i][j].x + 25) && (pFernand->y + 35 + 1 > pSalade[i][j].y) && (pFernand->y + 35 + 1 < pSalade[i][j].y + 25))
							ret = 1;
					}

				}
				if (key[RIGHT] == true)// on va à droite
				{
					if (pFernand->x + 16 > pSalade[i][j].x && pFernand->x + 16 < pSalade[i][j].x + 25 && pFernand->y + 35 >= pSalade[i][j].y && pFernand->y + 35 <= pSalade[i][j].y + 25)
						ret = 1;
				}
				else
				{
					if (key[LEFT] == true)// on va a gauche
					{
						if (pFernand->x - 1 > pSalade[i][j].x && pFernand->x - 1 < pSalade[i][j].x + 25 && pFernand->y + 35 >= pSalade[i][j].y && pFernand->y + 35 <= pSalade[i][j].y + 25)
							ret = 1;
					}
				}

			}
	return ret;
}

