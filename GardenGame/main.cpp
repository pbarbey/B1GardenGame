// Template ALLEGRO.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include "main.h"

#include "Salade.h"
#include "Personnage.h"
#include "limace.h"

#define NB_LIMACE_DROITE 3 
#define NB_LIMACE_GAUCHE 3 
#define NB_LIMACE 3


void erreur(const char* txt)
{
	ALLEGRO_DISPLAY* d;
	d = al_is_system_installed() ? al_get_current_display() : NULL;
	al_show_native_message_box(d, "ERREUR", txt, NULL, NULL, 0);
	exit(EXIT_FAILURE);

}

void message(const char* format, ...) //pouvoir afficher un message dans la fenetre allegro
{
	ALLEGRO_DISPLAY* display;
	char txt[1024]; //crée un tableau comportant 1024 caractères
	va_list args; // crée une liste d'arguments

	va_start(args, format); // crée une liste de param pour les arguments

	vsnprintf(txt, sizeof(txt), format, args);//récupère la chaine selon les param dans le tableau char txt

	va_end(args); //supprime la liste d'arguments

				  //récupére le display si il esxiste
	display = al_is_system_installed() ? al_get_current_display() : NULL;

	//affiche une fenetre de dialogue
	al_show_native_message_box(display, "TITRE general", "Titre message", txt, NULL, 0);

}
int main()
{
	// Pointeurs
	ALLEGRO_DISPLAY* display;
	ALLEGRO_BITMAP* pdecor, * conteneur, * allDroit, * allGauche, * animDroite[NBIMAGE], * animGauche[NBIMAGE];
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_TIMER* timer, * timer2, * timer3;
	SALADE TabSalade[10][4];
	int NumImageLimace = 0;

	t_sprite* limaceDroite[3];
	t_sprite* limaceGauche[3];


	ALLEGRO_BITMAP* cote_up[NBIMAGE], * cote_right[NBIMAGE], * cote_down[NBIMAGE], * cote_left[NBIMAGE];
	int cpt_up, cpt_right, cpt_down, cpt_left;
	int   dirEnCours = MONTE;
	char nom[256];

	srand(time(NULL));

	// Variables globales
	bool fin = false, dessin = false;
	int i;
	int highscore = 0;
	std::string hgs;

	//initialisation des addons pour le highscore
	al_init_font_addon();
	al_init_ttf_addon();

	// Touches de déplacements
	bool key[KEY_MAX] = { false,false,false,false };
	bool ret;

	// Initialisations
	if (!al_init())
		erreur("Allegro");

	if (!al_install_keyboard()) //prendre le clavier
		erreur("Clavier");

	queue = al_create_event_queue(); // créer la file dévénement
	if (!queue)
		erreur("evenement");

	timer = al_create_timer(1.0 / 60); // mettre un timer dans la file d'événement 
	if (!timer)
		erreur("al_create_timer()");

	timer2 = al_create_timer(3.0);//timer pour la réapparition des salades
	if (!timer2)
		erreur("timer2");

	timer3 = al_create_timer(0.1);//timer pour l'affichage des images du personnage
	if (!timer3)
		erreur("timer3");

	if (!al_init_image_addon()) //prendre une image
		erreur("al_init_image_addon()");

	//paramétrage fenetre avant sa creation
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	//plein écran sans fenetre
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN);


	display = al_create_display(800, 600); //créer un écran de 800 par 600
	if (!display)
		erreur("display");

	pdecor = al_load_bitmap("anim/carte.png"); //charger l'image
	if (!pdecor)
		erreur("al_load_bitmap() fond");

	//récupération de la police
	ALLEGRO_FONT* font = al_load_font("arial.ttf", 20, 0);

	//récupération de l'image de sprite limace droite
	allDroit = al_load_bitmap("anim/limace_droit.PNG");
	if (!allDroit)
		erreur("chargement sprite");

	//récupération de l'image de sprite limace gauche
	allGauche = al_load_bitmap("anim/limace_gauche.png");
	if (!allGauche)
		erreur("chargement sprite");

	//découpe du sprite limace droit
	for (i = 0; i < NBIMAGE; i++)
	{
		animDroite[i] = recup_sprite(allDroit, IMTX, IMTY, i * 100, 0, 4, i);
		if (!animDroite[i])
			erreur("recup du sprite droit");
		al_convert_mask_to_alpha(animDroite[i], al_get_pixel(animDroite[i], 0, 0));
	}

	//découpe du sprite limace gauche
	for (i = 0; i < NBIMAGE; i++)
	{
		animGauche[i] = recup_sprite(allGauche, IMTX, IMTY, i * 100, 0, 4, i);
		if (!animGauche[i])
			erreur("recup du sprite gauche");
		al_convert_mask_to_alpha(animGauche[i], al_get_pixel(animGauche[i], 0, 0));
	}


	//revenir à l'affichage de l'écran
	al_set_target_backbuffer(display);

	//allocation et initialisation d'un t_sprite droite
	for (i = 0; i < NB_LIMACE_DROITE; i++)
	{
		limaceDroite[i] = init_sprite(800, -1.0);
	}

	//allocation et initialisation d'un t_sprite gauche
	for (i = 0; i < NB_LIMACE_GAUCHE; i++)
	{
		limaceGauche[i] = init_sprite(0, 1.0);
	}

	// initialisation personnage. Attention les images doivent
// correspondre aux directions dans l'ordre
	for (i = 0; i < NBIMAGE; i++) {
		sprintf_s(nom, "anim/fernand/Fernandface%d.png", i);
		cote_down[i] = al_load_bitmap(nom);
		if (!cote_down[i])
			erreur("fernand face");
		al_convert_mask_to_alpha(cote_down[i], al_get_pixel(cote_down[i], 0, 0));

	}
	cpt_down = 0;

	// initialisation personnage. Attention les images doivent
	// correspondre aux directions dans l'ordre
	for (i = 0; i < NBIMAGE; i++) {
		sprintf_s(nom, "anim/fernand/Fernanddos%d.png", i);
		cote_up[i] = al_load_bitmap(nom);
		if (!cote_up[i])
			erreur("fernand dos");
		al_convert_mask_to_alpha(cote_up[i], al_get_pixel(cote_up[i], 0, 0));

	}
	cpt_up = 0;
	// initialisation personnage. Attention les images doivent
	// correspondre aux directions dans l'ordre
	for (i = 0; i < NBIMAGE; i++) {
		sprintf_s(nom, "anim/fernand/Fernanddroite%d.png", i);
		cote_right[i] = al_load_bitmap(nom);
		if (!cote_right[i])
			erreur("fernand droite");
		al_convert_mask_to_alpha(cote_right[i], al_get_pixel(cote_right[i], 0, 0));

	}
	cpt_right = 0;
	// initialisation personnage. Attention les images doivent
	// correspondre aux directions dans l'ordre
	for (i = 0; i < NBIMAGE; i++) {
		sprintf_s(nom, "anim/fernand/Fernandgauche%d.PNG", i);
		cote_left[i] = al_load_bitmap(nom);
		if (!cote_left[i])
			erreur("fernand gauche");
		al_convert_mask_to_alpha(cote_left[i], al_get_pixel(cote_left[i], 0, 0));

	}
	cpt_left = 0;


	image_personnage Personnage1;
	Personnage1.x = PosDepartx;
	Personnage1.y = PosDeparty;


	RecupSalade(TabSalade);

	al_register_event_source(queue,//la file d'événement
		al_get_display_event_source(display)); //sélection de l'écran
	al_register_event_source(queue,
		al_get_keyboard_event_source()); //sélection du clavier
	al_register_event_source(queue,
		al_get_timer_event_source(timer));//sélection du timer
	al_register_event_source(queue,
		al_get_timer_event_source(timer2));//sélection du timer des salades
	al_register_event_source(queue,
		al_get_timer_event_source(timer3));//sélection du timer des images


	al_start_timer(timer);
	al_start_timer(timer2);
	al_start_timer(timer3);
	while (!fin)
	{

		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event); //attendre un événment pour faire l'action

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			fin = true;

		// Déplacements

		if (event.type == ALLEGRO_EVENT_KEY_DOWN) //touche enfocée
		{
			switch (event.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				key[UP] = true;
				dirEnCours = MONTE;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[RIGHT] = true;
				dirEnCours = DROITE;
				break;

			case ALLEGRO_KEY_DOWN:
				key[DOWN] = true;
				dirEnCours = DESCEND;
				break;

			case ALLEGRO_KEY_LEFT:
				key[LEFT] = true;
				dirEnCours = GAUCHE;
				break;

			case ALLEGRO_KEY_ESCAPE:
				fin = true;
				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_KEY_UP) // les touches relevées
		{
			switch (event.keyboard.keycode) //événement du clavier correspondant au code d'une touche
			{
			case ALLEGRO_KEY_UP:
				key[UP] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[RIGHT] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				key[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				key[LEFT] = false;
				break;
			}
		}
		else if (event.type == ALLEGRO_EVENT_TIMER)
		{
			if (event.timer.source == timer2)
				PousseSalade(TabSalade);

			ret = CollisionFernandSalade(&Personnage1, TabSalade, key);
			if (!ret)
			{
				Personnage1.y -= key[UP] * 2;
				Personnage1.x += key[RIGHT] * 2;
				Personnage1.y += key[DOWN] * 2;
				Personnage1.x -= key[LEFT] * 2;
			}
			if (event.timer.source == timer3)
			{
				if (key[UP])
				{
					cpt_up += 1;
					if (cpt_up >= 4)
						cpt_up = 0;
				}

				else if (key[RIGHT])
				{
					cpt_right += 1;
					if (cpt_right >= 4)
						cpt_right = 0;

				}

				else if (key[DOWN])
				{
					cpt_down += 1;
					if (cpt_down >= 4)
						cpt_down = 0;
				}

				else if (key[LEFT])
				{
					cpt_left += 1;
					if (cpt_left >= 4)
						cpt_left = 0;
				}
			}
			dessin = true;
		}

		if (dessin == true && al_is_event_queue_empty(queue))
		{
			for (i = 0; i < NB_LIMACE; i++)
			{
				//controle animation limace
				cntl_anim(limaceDroite[i]);
				cntl_anim(limaceGauche[i]);
				//avancer limace
				CollisionLimaceSaladeDroite(limaceDroite[i], TabSalade);
				CollisionLimaceSaladeGauche(limaceGauche[i], TabSalade);
				ret = CollisionFernandLimaceDroite(limaceDroite[i], &Personnage1, key);
				ret = CollisionFernandLimaceGauche(limaceGauche[i], &Personnage1, key);
				if (ret == true) 
					highscore += 10;
				avance(limaceDroite[i]);
				avance(limaceGauche[i]);
			}
			al_draw_bitmap(pdecor, 0, 0, 0);
			AfficheSalade(TabSalade);

			if (dirEnCours == MONTE)
				al_draw_bitmap(Personnage1.image = cote_up[cpt_up], Personnage1.x, Personnage1.y, 0);
			if (dirEnCours == DROITE)
				al_draw_bitmap(Personnage1.image = cote_right[cpt_right], Personnage1.x, Personnage1.y, 0);
			if (dirEnCours == DESCEND)
				al_draw_bitmap(Personnage1.image = cote_down[cpt_down], Personnage1.x, Personnage1.y, 0);
			if (dirEnCours == GAUCHE)
				al_draw_bitmap(Personnage1.image = cote_left[cpt_left], Personnage1.x, Personnage1.y, 0);


			for (i = 0; i < NB_LIMACE; i++)
			{
				al_draw_bitmap(animDroite[limaceDroite[i]->imcourante], limaceDroite[i]->x, limaceDroite[i]->y, 0);
				al_draw_bitmap(animGauche[limaceGauche[i]->imcourante], limaceGauche[i]->x, limaceGauche[i]->y, 0);

			}
			//afficher le highcore
			hgs = std::to_string(highscore);
			al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0,"Score: ");
			al_draw_text(font, al_map_rgb(255, 255, 255), 70, 0, 0,hgs.c_str());

			al_flip_display(); // affiche le vrai écran

			dessin = false;
		}
	}

	for (i = 0; i < NBIMAGE; i++)
		al_destroy_bitmap(cote_up[i]);
	for (i = 0; i < NBIMAGE; i++)
		al_destroy_bitmap(cote_right[i]);
	for (i = 0; i < NBIMAGE; i++)
		al_destroy_bitmap(cote_down[i]);
	for (i = 0; i < NBIMAGE; i++)
		al_destroy_bitmap(cote_left[i]);
	al_destroy_timer(timer); //détruit le timer
	al_destroy_timer(timer2); //détruit le timer2
	al_destroy_timer(timer3);// détruit le timer3 
	al_destroy_event_queue(queue); //détruit la file d'événement
	al_destroy_display(display); // détruit l'écran d'affichage

	return 0;
}









