#include"solaire.h"
#define _USE_MATH_DEFINES
#include<math.h> /*pour cosinus*/
#include <cmath>
#include<iostream>
#include<fstream>
using namespace std;

const float PI = M_PI;


//ANAIS JULES
//2 FONCTIONS
float Ener_sol(int j, int nbj)
{
	//Lexique locale
	float energie, coss;


	//Algorithme locale
	coss = cos((360 * (j - 2.7206) / nbj)*PI / 180);//multiplier par pi/180 car calcul en degrés et on code en radian
	energie = 1367 * (1 + 0.0334*coss);
	return energie;
}


float hhmmss2dec(int hh, int mm, int ss)
{
	float res;
	// pour 18h48'24''
	res = hh + (((float)mm / 60.0) + ((float)ss / 3600.0));
	// 18h48'24'' donne donc 18,80667
	return res;
}

float Rendement(float a)//Réalisée par les chefs de projet
{
	float res, sous;
	res = 0.2*a;//Rendement du panneau solaire
	res = 0.97*res;//Conversion DC vers AC;
	res = 0.99*res;//Perte dans les fils
	return res;
}



//LOUIS GAEL ANTOINE
//3 FONCTIONS
//Définition rayonnement_solaire_direct
float rayonnement_solaire_direct(float esol, float er, float m, float tl)
{
	//Lexique local à rayonnement_solaire_direct
	float i;
	//Algorithme local à rayonnement_solaire_direct
	i = esol * exp(-er * m*tl);
	return i;
}


//Définition de mois_jourtojour
int Mois_jourtojour(int j, int m, int a)
{
	//Lexique local à mois_jourtojour
	const unsigned int nb_jours_mois[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	const unsigned int nb_jours_mois_bis[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
	int bis;
	int numero_jour;
	//Algorithme local à mois_jourtojour
	bis = Bissect(a);
	numero_jour = 0;
	if (bis == 366)
	{
		for (unsigned int i = 0; i < m - 1; i++)
		{
			numero_jour = numero_jour + nb_jours_mois_bis[i];
		}
	}
	else
	{
		for (unsigned int i = 0; i < m - 1; i++)
		{
			numero_jour = numero_jour + nb_jours_mois[i];
		}
	}
	numero_jour = numero_jour + j;
	return numero_jour;
}


degsex Deg2sex(float degdec)
{
	//Lexique local
	float part_dec, sec, deg, min;
	degsex loc;
	//Algorithme local
	deg = floor(degdec); //deg prend pour valeur la partie entière de degdec
	part_dec = degdec - deg;
	min = part_dec * 60;
	min = floor(min);
	sec = ((part_dec * 60) - min) * 60;
	loc.deg = deg;
	loc.min = min;
	loc.sec = sec;
	return loc;
}



double Temps_solaire(int annee, int jour, float hLegale, float longitudeRad) //Conversion heure légale en temps solaire vrai , heure légale en HEURE et longitude en RADIAN
{
	//Lexique
	//Correction de l'équation du temps
	double tetaj, muj;
	double equaTemps;
	//Correction de la longitude
	double correctLongitude;
	//Correction changement d'heure
	int j, je, jh;
	int jourEte, jourHiver;
	int correctChangementHeure;
	//Resultats
	double hSolaire = 0;



	//Algorithme

	hLegale = hLegale * 60; //On travail en minute dans cette fonction

	//Equation du temps E = C x O
	//Definition de tetaj et muj
	tetaj = 357.5291 + 0.98560028 * jour;
	muj = 280.4665 + 0.98564736 * jour;
	//Equation du temps 
	equaTemps = (7.6592 * sin(tetaj * PI / 180) + 0.08 * sin(2 * tetaj * PI / 180) + 0.0012 * sin(3 * tetaj * PI / 180)) + (-9.872 * sin(2 * muj * PI / 180) + 0.212 * sin(4 * muj * PI / 180) - 0.0056 * sin(6 * muj * PI / 180));


	//Correction de la longitude
	correctLongitude = 4 * (longitudeRad * 180 / PI);

	//Correction heure d'été ou heure d'hiver
	//Algorythme de Mike Keith pour déterminer un jour d'une semaine pour une année de 1583 à 9999
	//Trouver le dernier dimanche de mars
	je = 24;
	do
	{
		je++;
		j = (je + annee + annee / 4 - annee / 100 + annee / 400 + (31 * 1) / 12) % 7;
	} while (j != 0);
	//Trouver le dernier dimanche d'octobre
	jh = 24;
	do
	{
		jh++;
		j = (jh + annee + annee / 4 - annee / 100 + annee / 400 + (31 * 8) / 12) % 7;
	} while (j != 0);

	//Conversion du jour du mois en jour de l'année selon l'année bissextile ou non
	if (Bissect(annee) == 365)
	{
		jourEte = 59 + je;      //Nombre de jour jusqu'au 28 Février + nombre de jour jusqu'au dernier dimanche de mars
		jourHiver = 273 + jh;   //Nombre de jour jusqu'au 30 Septembre + nombre de jour jusqu'au dernier dimanche d'Octobre
	}
	else
	{
		jourEte = 60 + je;
		jourHiver = 274 + jh;
	}

	//Ajout ou retrait du nombre d'heure
	//Si le jour est strictement un jour a heure d'été
	if ((jour == jourEte && hLegale >= 120) || (jour == jourHiver && hLegale < 180) || ((jour > jourEte) && (jour < jourHiver)))
	{
		correctChangementHeure = 120; //On retire 2h à l'heure légale
	}
	else
	{
		correctChangementHeure = 60; //On retire 1h à l'heure légale
	}

	hSolaire = (hLegale - equaTemps - correctLongitude - correctChangementHeure) / 60; //Correction et conversion en heure //FAUTE

	//Si les modifications des heures entrainent une modification de la date légale
	if (hSolaire < 0) //Si l'heure est négative on se place au jour précédent
	{
		hSolaire = 24 + hSolaire; //Hsolaire est négatif
	}
	if (hSolaire > 24) //Si l'heure dépasse une journée, on se place au jour suivant
	{
		hSolaire = hSolaire - 24;
	}


	return hSolaire;
}






//JD JESSIE QUENTIN
//3 FONCTIONS
float Rayonnement_solaire_global(float i, float ci, float d)
{
	//lexique local
	float g;
	//algo local
	g = i * ci + d;
	return g;
}


float Vitesse_angulaire(int nbj)
{
	float res;
	res = (float)360 / nbj;
	return res;
}


int Bissect(int annee)

{
	//Lexique
	int nbj;

	//Algorithme

	if ((annee % 100 != 0 && annee % 4 == 0) || (annee % 400 == 0 && annee % 4000 != 0))

	{
		nbj = 366;
	}
	else
	{
		nbj = 365;
	}
	return nbj;
}


//DANY JEAN MAXIME
//4 FONCTIONS
double Pression_partielle_vap_eau(float hr, double pvs)
{
	//Lexique local à Pression_partielle_vap_eau
	double p_vap_eau;

	//Algorithme local à Pression_partielle_vap_eau

	p_vap_eau = pvs * hr;

	return p_vap_eau;
}


double Pression_partielle_vap_sat(float t)
{
	//Lexique local à Pression_partielle_vap_sat

	double p_vap_sat;

	//Algorithme local à Pression_partielle_vap_sat

	p_vap_sat = 2.165*pow((1.098 + t / 100), 8.02); //attention au #include<math.h>, fonctionne avec des exposants décimaux

	return p_vap_sat;
}

double Epaisseur_optique(float m)
{
	// Lexique local à Epaisseur_optique

	double ep_opt;


	//Algorithme local à Epaisseur optique
	ep_opt = 1 / (0.9*m + 9.4);

	return ep_opt;
}

float Sex2deg(int deg, int min, int sec)
{
	//Lexique local à Sex2deg
	float degres, total_sec;  //total_sec déclaré comme un float pour effectuer une division réelle

	//Algorithme local à Sex2deg

	total_sec = 60 * min + sec; // Nombre total de secondes
	degres = deg + total_sec / 3600; //  1 seconde est un 3600e de degré
	return degres;
}


//VALENTINE HUGO ALBI
//3 FONCTIONS
float coefficient_incidence(float h, float o, float a, float i)
{
	float res;
	res = sin(i)*cos(h)*cos(o - a) + cos(i)*sin(h);
	return res;







}


//Définition de la fonction
float rayonnement_solaire_diffus(float h, float i)

{
	//Lexique local

	float res;

	//Algorithme local

	res = 125 * (pow(sin(h), 0.4))*((1 + cos(i)) / 2) + 211.86*(pow(sin(h), 1.22))*((1 - cos(i)) / 2);

	return res;
}


//Défintion de la fonction
float Azimut(float dec, float ah, float h)

{
	//Lexique local
	float az;


	//Algorithme local
	az = asin((cos(dec)* (sin(ah))) / (cos(h)));
	if (az > 0)
	{
		if (ah < 0)
		{
			az = -az;
		}
	}
	else
	{
		if (ah > 0)
		{
			az = -az;
		}
	}

	return az;
}

//YANNIS CLAUDE LILIAN
//4 FONCTIONS
float Facteur_trouble(int c, float pv)
{
	float b;
	switch (c)
	{
	case 1:
		b = 0.2;
		break;
	case 2:
		b = 0.1;
		break;
	case 3:
		b = 0.05;
		break;
	default:
		b = 0.02;
	}
	return 2.4 + 14.6*b + 0.4*(1 + 2 * b)*log(pv);
}

float pression_atm(int z)
{
	//lexique 
	float patm;
	float x;
	float y;
	//algo
	x = 1 - 2.26*0.00001*z;
	y = pow(x, 5.26);
	patm = 101325 * y;
	return patm;
}

float Rad2deg(float angle)
{
	return (angle * 360) / (2 * PI);
}

float Masse_air_optique(float h, float patm)
{
	float n, m, power;
	n = 3.885 + (h * (180 / PI));
	power = pow(n, -1.253);
	m = patm / (101325 * sin(h) + 15198.75*power);
	return m;
}
//MATHIS PIERRE CHARLES

float Hauteur_soleil(float ah, float lat, float dec)
{
	float Hs;
	Hs = asin(sin(lat)*sin(dec) + cos(lat) * cos(dec) * cos(ah)); //calcule La hauteur solaire 
	return Hs;
}

float Angle_horaire(float tsv)
{
	float Ah;
	Ah = 180 * (tsv / 12 - 1); //calcule l'angle horaire
	return Ah;
}

double Declinaison(float va, int j)
{
	double Dec;
	Dec = asin(0.3978*sin((va * (j - 81 + 2 * sin((va*(j - 2)))))));//calcule la déclinaison solaire


	return Dec;
}

float Deg2rad(float deg)
{
	float rad;
	//convertit les angles en degré en radian
	rad = (deg*PI) / 180;
	return rad;
}




void langue(void)
{
	int language;

	cout << "Veuillez choisir une langue pour le projet d'informatique/Please choose a language" << endl;
	cout << "for the informatics project/Elige un idioma para el proyecto de inform\xA0tica" << endl;
	cout << "1.            -Fran" << "\x87" << "ais" << endl;
	cout << "2.            -English" << endl;
	cout << "3.            -Espa\xA4ol" << endl;
	cin >> language;
	switch (language)
	{
	case 1: menu_francais();
		break;
	case 2: menu_english();
		break;
	case 3: menu_espanol();
		break;
	default: cout << "Error" << endl;
		cout << "Retour au menu/Return to menu/Volver al menu" << endl;
		cout << endl;
		langue();
	}
}


void menu_francais(void)
{
	int a;
	int rtr;
	cout << endl;
	cout << "Bienvenue dans le projet d'informatique." << endl;
	cout << "Que souhaitez vous faire?" << endl;
	cout << "1.            -Visualiser le rayonnement solaire global re\x87u en un point du globe, un jour de l'ann" << "\x82" << "e" << endl;
	cout << "               a une heure donn" << "\x82" << "e." << endl;
	cout << "2.            -G\x82n\x82rer un fichier .csv contenant le rayonnement global re\x87u en un point du globe en fonction" << endl;
	cout << "               du jour de l'ann" << "\x82" << "e." << endl;
	cout << "3.            -G\x82n\x82rer un fichier .csv contenant le rayonnement global re\x87u en un point du globe pour un jour" << endl;
	cout << "               de l'ann" << "\x82" << "e et en fonction de l'heure de la journ" << "\x82" << "e." << endl;
	cout << "4.            -Savoir l'orientation et l'inclinaison optimale pour un capteur." << endl;
	cin >> a;
	switch (a)
	{
	case 1: Calculer_rayonnement_global_fr();//Actionne la fonction correspondante, calcule le rayonnement global
		break;
	case 2: Générer_fichier_jours_fr();//Actionne la fonction correspondante, génère un fichier en fonction du jour
		break;
	case 3: Générer_fichier_heures_fr();//Actionne la fonction correspondante, génère un fichier en fonction de l'heure
		break;
	case 4: Calculer_rayonnement_global_fr_or_incli();//Actionne l'optimisation de l'orientation et l'inclinaison et génère deux fichiers(voir bas de page)
		break;
	default:    cout << "Erreur, voulez vous revenir au menu?" << endl << "1- Oui" << endl << "Autre entier- Quitter le projet" << endl;
		cin >> rtr;//Si l'utilisateur se trompe il peut revenir au projet ou le quitter
		if (rtr == 1)
		{
			cout << endl;
			menu_francais();
		}
		else
		{
		}
	}
}

void menu_english(void)
{
	int a;
	int rtr;
	cout << endl;
	cout << "Welcome to the informatics project" << endl;
	cout << "What would you like to do?" << endl;
	cout << "1.            -Visualize the global solar radiation received at a certain point of the globe, one day of the year" << endl;
	cout << "               at a certain hour." << endl;
	cout << "2.            -Generate a csv file containing the global solar radiation received at a certain point of the globe" << endl;
	cout << "               depending on the day of the year." << endl;
	cout << "3.            -Generate a csv file containing the global solar radiation received at a certain point of the globe " << endl;
	cout << "               for a day of the year and depending on the time of the day." << endl;
	cout << "4.            -Know the optimal orientation and inclination for a solar captor." << endl;
	cin >> a;
	switch (a)
	{
	case 1: Calculer_rayonnement_global_en();
		break;
	case 2: Générer_fichier_jours_en();
		break;
	case 3: Générer_fichier_heures_en();
		break;
	case 4: Calculer_rayonnement_global_en_or_incli();//Actionne l'optimisation de l'orientation et l'inclinaison et génère deux fichiers(voir bas de page)
		break;
	default:    cout << "Error, would you like to return to the menu?" << endl << "1- Yes" << endl << "Other integer- Quit the project" << endl;
		cin >> rtr;
		if (rtr == 1)
		{
			cout << endl;
			menu_english();
		}
		else
		{
		}
	}
}

void menu_espanol(void)
{
	int a;
	int rtr;
	cout << endl;
	cout << "Bienvenido en el proyecto de inform\xA0tica." << endl;
	cout << "Que quer\x82is hacer?" << endl;
	cout << "1.            -Visualizar la radiaci\xA2n solar global recibida en un punto del globo, un d" << "\xA1" << "a del a\xA4o" << endl;
	cout << "               a una hora determinada." << endl;
	cout << "2.            -Generar un archivo .csv que contiene la radiaci\xA2n solar global recibida en un punto del globo" << endl;
	cout << "               en funci\xA2n del d" << "\xA1" << "a del a\xA4o." << endl;
	cout << "3.            -Generar un archivo .csv que contiene la radiaci\xA2n solar global recibida en un punto del globo" << endl;
	cout << "               para un d" << "\xA1" << "a del a\xA4o y en funci\xA2n de la hora del d" << "\xA1" << "a." << endl;
	cout << "4.            -Saber la orientaci"<<"\xA2"<<"n y la inclinaci" << "\xA2" << "n optim" << "\xA0" << "s para un colector solar." << endl;
	cin >> a;
	switch (a)
	{
	case 1: Calculer_rayonnement_global_es();
		break;
	case 2: Générer_fichier_jours_es();
		break;
	case 3: Générer_fichier_heures_es();
		break;
	case 4: Calculer_rayonnement_global_es_or_incli();//Actionne l'optimisation de l'orientation et l'inclinaison et génère deux fichiers(voir bas de page)
		break;
	default:    cout << "Error, quer\x82is volver al men\xA3?" << endl << "1- S\xA1" << endl << "Otro entero- Dejar el proyecto" << endl;
		cin >> rtr;
		if (rtr == 1)
		{
			cout << endl;
			menu_espanol();
		}
		else
		{
		}
	}
}

void Calculer_rayonnement_global_fr(void)
{
	//Declaration de toutes les variables.
	int choix;//Choix, que faire à la fin de la fonction


	int d;//variables pour rentrer tous les paramètres
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;//variables pour rentrer tous les paramètres

	float longituderadian;//Variables servant à stocker les paramètres calculés par les fonctions
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	cout << endl;
	cout << "Choisissez votre environnement" << endl;
	cout << "1. Environnement industriel" << endl;
	cout << "2. Environnement urbain" << endl;
	cout << "3. Environnement rural" << endl;
	cout << "4. Environnement montagnard" << endl;
	cin >> environnement;
	cout << endl << "Entrez un r" << "\x82" << "el, la temp\x82rature en degr\x82s celsius et un r" << "\x82" << "el, le taux moyen d'humidit\x82 relative" << endl;
	cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
	cout << endl;
	cout << "Comment voulez vous rentrer les donn" << "\x82" << "es angulaires?" << endl;
	cout << " 1- En degr\x82s d" << "\x82" << "cimaux" << endl;
	cout << " autre entier- En degr\x82s sexag\x82simaux" << endl;//Choix donné à l'utilisateur pour les coordonnées sphériques
	cin >> d;
	if (d == 1)
	{
		cout << endl << "Entrez la latitude en degr\x82s d" << "\x82" << "cimaux, la longitude en degr\x82s d" << "\x82" << "cimaux ainsi que l'altitude ( 1 entier) en m\x8Atres" << endl;
		cin >> latitudedegré >> longitudedegré >> altitude;
	}
	else
	{
		cout << endl << "Entrez la latitude en degr\x82s sexag\x82simaux, la longitude en degr\x82s sexag\x82simaux ainsi que l'altitude (1 entier) en m\x8Atres" << endl;
		cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
		latitudedegré = Sex2deg(la1, la2, la3);
		longitudedegré = Sex2deg(lo1, lo2, lo3);
	}

	cout << endl << "Entrez une date : ann" << "\x82" << "e, mois, jour" << endl;
	cin >> année >> mois >> jour;
	cout << endl << "Entrez heure, minute, seconde" << endl;
	cin >> heure >> minute >> seconde;
	cout << endl << "Entrez l'orientation puis l'inclinaison" << endl;
	cin >> orientation >> inclinaison;

	jièmejour = Mois_jourtojour(jour, mois, année);//Déterminer le jour de l'année en fonction de l'année, du mois et du jour
	nombrejourannée = Bissect(année);//Déterminer le nombre de jours dans l'année si année bissextile ou non
	vitesseangulaire = Vitesse_angulaire(nombrejourannée);
	vitesseangulaireradian = Deg2rad(vitesseangulaire);//conversion en radians
	déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

	heurelégale = hhmmss2dec(heure, minute, seconde);//Conversion en heure légale décimale

	longituderadian = Deg2rad(longitudedegré);//conversion en radians
	latituderadian = Deg2rad(latitudedegré);//conversion en radians
	//Calcul du temps solaire et de tous les paramètres en découlant
	tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
	anglehoraire = Angle_horaire(tempssolaire);
	anglehoraireradian = Deg2rad(anglehoraire);//conversion en radians
	hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);//Calcul de la hauteur solaire grâce
	// à tous les paramètres précédents

	//Calcul de la masse d'air optique relative
	pressionatmo = pression_atm(altitude);
	masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);
	//Calcul du facteur trouble de Linke
	pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
	pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
	facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

	epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
	energiesolaire = Ener_sol(jièmejour, nombrejourannée);
	//Calcul du rayonnement solaire direct grâce à tous les paramètres calculés précedemment
	rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);

	//Calcul du rayonnement solaire diffus
	inclinaisonradian = Deg2rad(inclinaison);
	rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);
	//Correction si la hauteur solaire est inférieure à zero, le soleil est couché, cad pas de rayonnement.
	if (hauteursoleilradian < 0)
	{
		rayonnementsolairedirect = 0;
		rayonnementsolairediffus = 0;
	}
	//Calcul du coefficient d'incidence
	azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
	orientationradian = Deg2rad(orientation);
	coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);
	//Calcul du rayonnement solaire global
	rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

	cout << endl << endl << "Le rayonnement est de " << rayonnementsolaireglobal << "Watts par m\x8Atre carr\x82" << endl << endl;
	cout << endl << "Le rayonnement utile est de " << Rendement(rayonnementsolaireglobal) << "Watts par m\x8Atre carr\x82" << endl;//Rayonnement utilisable
	cout << "Que souhaitez vous faire ?" << endl;
	cout << "1.            -G\x82n\x82rer un fichier .csv contenant le rayonnement global re" << "\x87" << "u en un point du globe en fonction" << endl;
	cout << "               du jour de l'ann" << "\x82" << "e avec les param" << "\x8A" << "tres pr" << "\x82" << "c" << "\x82" << "demment rentr" << "\x82" << "s." << endl;
	cout << "2.            -G\x82n\x82rer un fichier .csv contenant le rayonnement global re" << "\x87" << "u en un point du globe pour un jour" << endl;
	cout << "               de l'ann" << "\x82" << "e et en fonction de l'heure de la journ" << "\x82" << "e avec les param" << "\x8A" << "tres pr" << "\x82" << "c" << "\x82" << "demment rentr" << "\x82" << "s." << endl;
	cout << "3.            -G\x82n\x82rer les deux fichiers pr" << "\x82" << "c" << "\x82" << "dents avec les param" << "\x8A" << "tres pr" << "\x82" << "c" << "\x82" << "demment rentr" << "\x82" << "s. " << endl;
	cout << "Autre entier  -Retour au menu." << endl;
	cin >> choix;//Choix de générer des fichiers sans rentrer à nouveau les paramètres
	switch (choix)
	{
	case 1:
		Générer_fichier_jours_fr_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde, orientation, inclinaison);
		menu_francais();
		break;
	case 2:
		Générer_fichier_heures_fr_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour, orientation, inclinaison);
		menu_francais();
		break;
	case 3:
		Générer_fichier_jours_fr_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde, orientation, inclinaison);
		Générer_fichier_heures_fr_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour, orientation, inclinaison);

		menu_francais();
		break;
	default:
		menu_francais();
	}

}

void Calculer_rayonnement_global_en(void)
{
	int choix;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	cout << endl;
	cout << "Select an environment" << endl;
	cout << "1. Industrial environment" << endl;
	cout << "2. Urban environment" << endl;
	cout << "3. Rural environment" << endl;
	cout << "4. Mountain environment" << endl;
	cin >> environnement;
	cout << endl << "Enter 1 real number, the temperature in celsius degrees and an integer, the average relative humidity" << endl;
	cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
	cout << "How would you like to input the angular data?" << endl;
	cout << " 1- In decimal degrees" << endl;
	cout << " other integer- In sexagesimal degrees" << endl;
	cin >> d;
	if (d == 1)
	{
		cout << endl << "Enter the latitude in decimal degrees, the longitude in decimal degrees and the height ( 1 integer) in meters" << endl;
		cin >> latitudedegré >> longitudedegré >> altitude;
	}
	else
	{
		cout << endl << "Enter the latitude in sexagesimal degrees, the longitude in sexagesimal degrees and the height ( 1 integer) in meters" << endl;
		cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
		latitudedegré = Sex2deg(la1, la2, la3);
		longitudedegré = Sex2deg(lo1, lo2, lo3);
	}

	cout << endl << "Enter a date : year, month, day" << endl;
	cin >> année >> mois >> jour;
	cout << endl << "Enter hour, minute, second" << endl;
	cin >> heure >> minute >> seconde;
	cout << endl << "Enter the orientation then the inclination" << endl;
	cin >> orientation >> inclinaison;

	jièmejour = Mois_jourtojour(jour, mois, année);
	nombrejourannée = Bissect(année);

	pressionatmo = pression_atm(altitude);
	vitesseangulaire = Vitesse_angulaire(nombrejourannée);
	vitesseangulaireradian = Deg2rad(vitesseangulaire);
	déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
	heurelégale = hhmmss2dec(heure, minute, seconde);

	longituderadian = Deg2rad(longitudedegré);
	latituderadian = Deg2rad(latitudedegré);


	tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
	anglehoraire = Angle_horaire(tempssolaire);
	anglehoraireradian = Deg2rad(anglehoraire);
	hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);



	masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

	pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
	pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
	facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

	epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
	energiesolaire = Ener_sol(jièmejour, nombrejourannée);
	rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);


	inclinaisonradian = Deg2rad(inclinaison);
	rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

	if (hauteursoleilradian < 0)
	{
		rayonnementsolairedirect = 0;
		rayonnementsolairediffus = 0;
	}

	déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
	azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
	orientationradian = Deg2rad(orientation);
	coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);
	rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
	cout << endl << endl << "The radiation is " << rayonnementsolaireglobal << " Watt per square meter" << endl << endl;
	cout << "The useful radiation is " << Rendement(rayonnementsolaireglobal) << "Watt per square meter" << endl << endl;
	cout << "What would you like to do?" << endl;
	cout << "1.            -Generate a csv file containing the global solar radiation received at a certain point of the globe" << endl;
	cout << "               depending on the day with the parameters previously inputed." << endl;
	cout << "2.            -Generate a csv file containing the global solar radiation received at a certain point of the globe" << endl;
	cout << "               on one day depending on the hour with the parameters previously inputed." << endl;
	cout << "3.            -Generate both csv files with the parameters previously inputed." << endl;
	cout << "Other integer -Return to menu." << endl;
	cin >> choix;
	switch (choix)
	{
	case 1:
		Générer_fichier_jours_en_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde, orientation, inclinaison);
		menu_english();
		break;
	case 2:
		Générer_fichier_heures_en_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour, orientation, inclinaison);
		menu_english();
		break;
	case 3:
		Générer_fichier_jours_en_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde, orientation, inclinaison);
		Générer_fichier_heures_en_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour, orientation, inclinaison);
		menu_english();
		break;
	default:
		menu_english();
	}


}

void Calculer_rayonnement_global_es(void)
{
	int choix;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	cout << endl;
	cout << "Elige un ambiante" << endl;
	cout << "1.   Ambiante industrial" << endl;
	cout << "2.   Ambiante urbano" << endl;
	cout << "3.   Ambiante rural" << endl;
	cout << "4.   Ambiante monta\xA4osa" << endl;
	cin >> environnement;
	cout << endl << "Entrar 1 n\xA3mero real, la temperatura en grados celsius y un n\xA3mero real, la humedad relativa media" << endl;
	cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
	cout << "C\xA2mo quer\x82is entrar los datos angulares?" << endl;
	cout << " 1- En grados decimales" << endl;
	cout << " Otro entero- En grados sexagesimales" << endl;
	cin >> d;
	if (d == 1)
	{
		cout << endl << "Entrar la latitud en grados decimales, la longitud en grados decimales y la altitud ( 1 entero) en metros" << endl;
		cin >> latitudedegré >> longitudedegré >> altitude;
	}
	else
	{
		cout << endl << "Entrar la latitud en grados sexagesimales, la longitud en grados sexagesimales y la altitud ( 1 entero) en metros" << endl;
		cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
		latitudedegré = Sex2deg(la1, la2, la3);
		longitudedegré = Sex2deg(lo1, lo2, lo3);
	}

	cout << endl << "Entrar una fecha : a\xA4o, mes, d" << "\xA1" << "a" << endl;
	cin >> année >> mois >> jour;
	cout << endl << "Entrar hora, minuto, segunda" << endl;
	cin >> heure >> minute >> seconde;
	cout << endl << "Entrar la orientaci\xA2n luego la inclinaci\xA2n" << endl;
	cin >> orientation >> inclinaison;

	jièmejour = Mois_jourtojour(jour, mois, année);
	nombrejourannée = Bissect(année);

	pressionatmo = pression_atm(altitude);
	vitesseangulaire = Vitesse_angulaire(nombrejourannée);
	vitesseangulaireradian = Deg2rad(vitesseangulaire);
	déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
	heurelégale = hhmmss2dec(heure, minute, seconde);

	longituderadian = Deg2rad(longitudedegré);
	latituderadian = Deg2rad(latitudedegré);


	tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
	anglehoraire = Angle_horaire(tempssolaire);
	anglehoraireradian = Deg2rad(anglehoraire);
	hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);



	masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

	pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
	pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
	facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

	epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
	energiesolaire = Ener_sol(jièmejour, nombrejourannée);
	rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);




	inclinaisonradian = Deg2rad(inclinaison);
	rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

	if (hauteursoleilradian < 0)
	{
		rayonnementsolairedirect = 0;
		rayonnementsolairediffus = 0;
	}

	déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
	azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
	orientationradian = Deg2rad(orientation);
	coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);
	rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
	cout << endl << endl << "La radiaci\xA2n es de " << rayonnementsolaireglobal << "vatios por metro cuadrado." << endl << endl;
	cout << "La radiaci\xA2n util es de " << Rendement(rayonnementsolaireglobal) << "vatios por metro cuadrado." << endl << endl;
	cout << "Que te gustar" << "\xA1" << "a hacer?" << endl;
	cout << "1.            -Generar un archivo csv que contiene la radiaci\xA2n solar global recibida en un punto del globo," << endl;
	cout << "               dependiendo del d" << "\xA1" << "a con los par" << "\xA0" << "metros ingresados previamente." << endl;
	cout << "2.            -Generar un archivo csv que contiene la radiaci\xA2n solar global recibida en un punto del globo en un d" << "\xA1" << "a," << endl;
	cout << "               dependiendo de la hora con los par" << "\xA0" << "metros ingresados previamente." << endl;
	cout << "3.            -Generar ambos archivos csv con los par" << "\xA0" << "metros ingresados previamente." << endl;
	cout << "Otro entero -Regresar al men" << "\xA3" << "." << endl;
	cin >> choix;
	switch (choix)
	{
	case 1:
		Générer_fichier_jours_es_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde, orientation, inclinaison);
		menu_espanol();
		break;
	case 2:
		Générer_fichier_heures_es_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour, orientation, inclinaison);
		menu_espanol();
		break;
	case 3:
		Générer_fichier_jours_es_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde, orientation, inclinaison);
		Générer_fichier_heures_es_par(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour, orientation, inclinaison);
		menu_espanol();
		break;
	default:
		menu_espanol();
	}
}


void Générer_fichier_jours_fr(void)
{
	int a;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("rayonnement_global_en_fonction_du_jour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions
		cout << "Choisissez votre environnement" << endl;
		cout << "1. Environnement industriel" << endl;
		cout << "2. Environnement urbain" << endl;
		cout << "3. Environnement rural" << endl;
		cout << "4. Environnement montagnard" << endl;
		cin >> environnement;
		cout << endl << "Entrez 1 r" << "\x82" << "el, la temp\x82rature en degr\x82s celsius et un r" << "\x82" << "el, le taux moyen d'humidit\x82 relative" << endl;
		cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
		cout << "Comment voulez vous rentrer les donn" << "\x82" << "es angulaires?" << endl;
		cout << " 1- En degr\x82s d" << "\x82" << "cimaux" << endl;
		cout << " autre entier- En degr\x82s sexag\x82simaux" << endl;
		cin >> d;
		if (d == 1)
		{
			cout << endl << "Entrez la latitude en degr\x82s d" << "\x82" << "cimaux, la longitude en degr\x82s d" << "\x82" << "cimaux ainsi que l'altitude ( 1 entier) en m\x8Atres" << endl;
			cin >> latitudedegré >> longitudedegré >> altitude;
		}
		else
		{
			cout << endl << "Entrez la latitude en degr\x82s sexag\x82simaux, la longitude en degr\x82s sexag\x82simaux ainsi que l'altitude (1 entier) en m\x8Atres" << endl;
			cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
			latitudedegré = Sex2deg(la1, la2, la3);
			longitudedegré = Sex2deg(lo1, lo2, lo3);
		}

		cout << endl << "Entrez une date : ann" << "\x82" << "e" << endl;
		cin >> année;
		cout << endl << "Entrez heure, minute, seconde" << endl;
		cin >> heure >> minute >> seconde;
		cout << endl << "Entrez l'orientation puis l'inclinaison" << endl;
		cin >> orientation >> inclinaison;

		jièmejour = 1;//On initialise au premier jour puis on rentre dans la boucle
		fichier << "jour,Rayonnement solaire global(W/m^2)" << endl;
		nombrejourannée = Bissect(année);
		while (jièmejour <= nombrejourannée)//On veut 365 valeurs ou 366 selon si l'année est bissextile ou non
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaison, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;//Pour chaque tour de boucle on renvoie le jour
			//et le rayonnement associé
			jièmejour = jièmejour + 1;//Condition pour que la boucle ne soit pas infinie
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Erreur à l'ouverture !" << endl;
	}
	cout << endl << "retour au menu";
	menu_francais();
}
void Générer_fichier_jours_en(void)
{
	int a;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("rayonnement_global_en_fonction_du_jour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions
		cout << "Select an environment" << endl;
		cout << "1. Industrial environment" << endl;
		cout << "2. Urban environment" << endl;
		cout << "3. Rural environment" << endl;
		cout << "4. Mountain environment" << endl;
		cin >> environnement;
		cout << endl << "Enter 1 real number, the temperature in celsius degrees and one real number, the average relative humidity" << endl;
		cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
		cout << "How would you like to input the angular data?" << endl;
		cout << " 1- In decimal degrees" << endl;
		cout << " other integer- In sexagesimal degrees" << endl;
		cin >> d;
		if (d == 1)
		{
			cout << endl << "Enter the latitude in decimal degrees, the longitude in decimal degrees and the height ( 1 integer) in meters" << endl;
			cin >> latitudedegré >> longitudedegré >> altitude;
		}
		else
		{
			cout << endl << "Enter the latitude in sexagesimal degrees, the longitude in sexagesimal degrees and the height ( 1 integer) in meters" << endl;
			cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
			latitudedegré = Sex2deg(la1, la2, la3);
			longitudedegré = Sex2deg(lo1, lo2, lo3);
		}

		cout << endl << "Enter a date : year" << endl;
		cin >> année;
		cout << endl << "Enter hour, minute, second" << endl;
		cin >> heure >> minute >> seconde;
		cout << endl << "Enter the orientation then the inclination" << endl;
		cin >> orientation >> inclinaison;

		jièmejour = 1;
		fichier << "day,Global radiation(W/m^2)" << endl;
		nombrejourannée = Bissect(année);
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaison, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error at the opening !" << endl;
	}
	cout << "Return to menu";
	menu_english();
}
void Générer_fichier_jours_es(void)
{
	int a;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("rayonnement_global_en_fonction_du_jour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions
		cout << "Elige un ambiante" << endl;
		cout << "1.   Ambiante industrial" << endl;
		cout << "2.   Ambiante urbano" << endl;
		cout << "3.   Ambiante rural" << endl;
		cout << "4.   Ambiante monta\xA4osa" << endl;
		cin >> environnement;
		cout << endl << "Entrar 1 n\xA3mero real, la temperatura en grados celsius y un n\xA3mero real, la humedad relativa media" << endl;
		cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
		cout << "C" << "\xA2" << "mo quer\x82is entrar los datos angulares?" << endl;
		cout << " 1- En grados decimales" << endl;
		cout << " Otro entero- En grados sexagesimales" << endl;
		cin >> d;
		if (d == 1)
		{
			cout << endl << "Entrar la latitud en grados decimales, la longitud en grados decimales y la altitud ( 1 entero) en metros" << endl;
			cin >> latitudedegré >> longitudedegré >> altitude;
		}
		else
		{
			cout << endl << "Entrar la latitud en grados sexagesimales, la longitud en grados sexagesimales y la altitud ( 1 entero) en metros" << endl;
			cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
			latitudedegré = Sex2deg(la1, la2, la3);
			longitudedegré = Sex2deg(lo1, lo2, lo3);
		}

		cout << endl << "Entrar una fecha : a\xA4o" << endl;
		cin >> année;
		cout << endl << "Entrar hora, minuto, segunda" << endl;
		cin >> heure >> minute >> seconde;
		cout << endl << "Entrar la orientaci\xA2n luego la inclinaci\xA2n" << endl;
		cin >> orientation >> inclinaison;

		jièmejour = 1;
		fichier << "día,Radiación global(W/m^2)" << endl;
		nombrejourannée = Bissect(année);
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaison, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error al abrir !" << endl;
	}

	cout << endl << endl << "Volver al menu";
	menu_espanol();
}

void Générer_fichier_heures_fr(void)
{
	int a;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	ofstream fichier2("rayonnement_global_en_fonction_de_l_heure.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{
		// instructions
		cout << "Choisissez votre environnement" << endl;
		cout << "1. Environnement industriel" << endl;
		cout << "2. Environnement urbain" << endl;
		cout << "3. Environnement rural" << endl;
		cout << "4. Environnement montagnard" << endl;
		cin >> environnement;
		cout << endl << "Entrez 1 r" << "\x82" << "el, la temp\x82rature en degr\x82s celsius et un r" << "\x82" << "el, le taux moyen d'humidit\x82 relative" << endl;
		cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
		cout << "Comment voulez vous rentrer les donn" << "\x82" << "es angulaires?" << endl;
		cout << " 1- En degr\x82s d" << "\x82" << "cimaux" << endl;
		cout << " autre entier- En degr\x82s sexag\x82simaux" << endl;
		cin >> d;
		if (d == 1)
		{
			cout << endl << "Entrez la latitude en degr\x82s d" << "\x82" << "cimaux, la longitude en degr\x82s d" << "\x82" << "cimaux ainsi que l'altitude ( 1 entier) en m\x8Atres" << endl;
			cin >> latitudedegré >> longitudedegré >> altitude;
		}
		else
		{
			cout << endl << "Entrez la latitude en degr\x82s sexag\x82simaux, la longitude en degr\x82s sexag\x82simaux ainsi que l'altitude (1 entier) en \x8Atres" << endl;
			cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
			latitudedegré = Sex2deg(la1, la2, la3);
			longitudedegré = Sex2deg(lo1, lo2, lo3);
		}

		cout << endl << "Entrez une date : ann" << "\x82" << "e,mois,jour" << endl;
		cin >> année >> mois >> jour;
		//cout << endl << "Entrez heure, minute, seconde" << endl;
		//cin >> heure >> minute >> seconde;
		cout << endl << "Entrez l'orientation puis l'inclinaison" << endl;
		cin >> orientation >> inclinaison;
		heure = 1;//On initialise à 1h pour aller jusqu'à 24h
		minute = 0;
		seconde = 0;
		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		fichier2 << "heure, rayonnementsolairedirect(W/m^2), rayonnementsolairediffus(W/m^2), rayonnementsolaireglobal(W/m^2)" << endl;
		while (heure <= 24)//On va jusqu'à 24h
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier2 << heure << "," << rayonnementsolairedirect << "," << rayonnementsolairediffus << "," << rayonnementsolaireglobal << "," << endl;

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
		cerr << "Erreur à l'ouverture!" << endl;

	cout << endl << "Retour au menu";
	menu_francais();

}
void Générer_fichier_heures_en(void)
{
	int a;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	ofstream fichier2("Global_radiation_depending_on_hour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{
		// instructions
		cout << "Select an environment" << endl;
		cout << "1. Industrial environment" << endl;
		cout << "2. Urban environment" << endl;
		cout << "3. Rural environment" << endl;
		cout << "4. Mountain environment" << endl;
		cin >> environnement;
		cout << endl << "Enter 1 real number, the temperature in celsius degrees and one real number, the average relative humidity" << endl;
		cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
		cout << "How would you like to input the angular data?" << endl;
		cout << " 1- In decimal degrees" << endl;
		cout << " other integer- In sexagesimal degrees" << endl;
		cin >> d;
		if (d == 1)
		{
			cout << endl << "Enter the latitude in decimal degrees, the longitude in decimal degrees and the height ( 1 integer) in meters" << endl;
			cin >> latitudedegré >> longitudedegré >> altitude;
		}
		else
		{
			cout << endl << "Enter the latitude in sexagesimal degrees, the longitude in sexagesimal degrees and the height ( 1 integer) in meters" << endl;
			cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
			latitudedegré = Sex2deg(la1, la2, la3);
			longitudedegré = Sex2deg(lo1, lo2, lo3);
		}

		cout << endl << "Enter a date : year, month, day" << endl;
		cin >> année >> mois >> jour;
		cout << endl << "Enter the orientation then the inclination" << endl;
		cin >> orientation >> inclinaison;

		heure = 1;
		minute = 0;
		seconde = 0;
		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		fichier2 << "hour, direct radiation(W/m^2), diffuse radiation(W/m^2), global radiation(W/m^2)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier2 << heure << "," << rayonnementsolairedirect << "," << rayonnementsolairediffus << "," << rayonnementsolaireglobal << endl;

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
		cerr << "Error at the opening !" << endl;

	cout << endl << "Return to menu";
	menu_english();

}
void Générer_fichier_heures_es(void)
{
	int a;
	char b;
	int c;
	int d;
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	ofstream fichier2("Radiación_global_según_hora.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{
		// instructions
		cout << "Elige un ambiante" << endl;
		cout << "1.   Ambiante industrial" << endl;
		cout << "2.   Ambiante urbano" << endl;
		cout << "3.   Ambiante rural" << endl;
		cout << "4.   Ambiante monta\xA4osa" << endl;
		cin >> environnement;
		cout << endl << "Entrar 1 n\xA3mero real, la temperatura en grados celsius y un n\xA3mero real, la humedad relativa media" << endl;
		cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
		cout << "C" << "\xA2" << "mo quer\x82is entrar los datos angulares?" << endl;
		cout << " 1- En grados decimales" << endl;
		cout << " Otro entero- En grados sexagesimales" << endl;
		cin >> d;
		if (d == 1)
		{
			cout << endl << "Entrar la latitud en grados decimales, la longitud en grados decimales y la altitud ( 1 entero) en metros" << endl;
			cin >> latitudedegré >> longitudedegré >> altitude;
		}
		else
		{
			cout << endl << "Entrar la latitud en grados sexagesimales, la longitud en grados sexagesimales y la altitud ( 1 entero) en metros" << endl;
			cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
			latitudedegré = Sex2deg(la1, la2, la3);
			longitudedegré = Sex2deg(lo1, lo2, lo3);
		}

		cout << endl << "Entrar una fecha : a\xA4o, mes, d" << "\xA1" << "a" << endl;
		cin >> année >> mois >> jour;
		//cout << endl << "Entrar hora, minuto, segunda" << endl;
		//cin >> heure >> minute >> seconde;
		cout << endl << "Entrar la orientaci\xA2n luego la inclinaci\xA2n" << endl;
		cin >> orientation >> inclinaison;

		heure = 1;
		minute = 0;
		seconde = 0;
		jièmejour = Mois_jourtojour(jour, mois, année);
		cout << endl << jièmejour << endl;

		nombrejourannée = Bissect(année);
		cout << nombrejourannée << endl;
		fichier2 << "hora, radiación directa(W/m^2), radiación difusa(W/m^2), radiación global(W/m^2)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);

			vitesseangulaireradian = Deg2rad(vitesseangulaire);

			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);


			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);



			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier2 << heure << "," << rayonnementsolairedirect << "," << rayonnementsolairediffus << "," << rayonnementsolaireglobal << endl;

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
		cerr << "Error al abrir!" << endl;

	cout << endl << endl << "Volver al men" << "\xA3" << endl;
	menu_espanol();

}



//Memes fonction que celles pour générer des paramètres mais avec les paramètres en entrée pour ne pas avoir à les saisir à nouveau
void Générer_fichier_jours_fr_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde, float orientation, float inclinaison)
{


	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("rayonnement_global_en_fonction_du_jour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = 1;

		nombrejourannée = Bissect(année);
		fichier << "jour,Rayonnement solaire global(W/m^2)" << endl;
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);


			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}

			azimutradian = Azimut(déclinaison, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Erreur à l'ouverture !" << endl;
	}
	cout << endl << "retour au menu";
}

void Générer_fichier_jours_en_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde, float orientation, float inclinaison)
{


	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("Global_radiation_depending_on_the_day.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = 1;

		nombrejourannée = Bissect(année);
		fichier << "day,Global radiation(W/m^2)" << endl;
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaison, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error at the opening!" << endl;
	}
	cout << endl << "Return to menu";

}

void Générer_fichier_jours_es_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde, float orientation, float inclinaison)
{


	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("Radiacion_global_segun_el_dia.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = 1;

		nombrejourannée = Bissect(année);
		fichier << "día,Radiación global(W/m^2)" << endl;
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaison = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaison);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaison, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error al abrir." << endl;
	}
	cout << endl << "Volver al men" << "\xA3" << "." << endl;

}


void Générer_fichier_heures_fr_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour, float orientation, float inclinaison)
{
	int heure;
	int minute;
	int seconde;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	ofstream fichier2("rayonnement_global_en_fonction_de_l_heure.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{



		heure = 1;
		minute = 0;
		seconde = 0;
		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		fichier2 << "heure, rayonnementsolairedirect(W/m^2), rayonnementsolairediffus(W/m^2), rayonnementsolaireglobal(W/m^2)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);

			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier2 << heure << "," << rayonnementsolairedirect << "," << rayonnementsolairediffus << "," << rayonnementsolaireglobal << "," << endl;

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Erreur à l'ouverture !" << endl;
	}

	cout << endl << "Retour au menu";

}

void Générer_fichier_heures_en_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour, float orientation, float inclinaison)
{
	int heure;
	int minute;
	int seconde;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	ofstream fichier2("Global_radiation_depending_on_hour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{



		heure = 1;
		minute = 0;
		seconde = 0;
		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		fichier2 << "hour, direct radiation(W/m^2), diffuse radiation(W/m^2), global radiation(W/m^2)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier2 << heure << "," << rayonnementsolairedirect << "," << rayonnementsolairediffus << "," << rayonnementsolaireglobal << endl;

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error at the opening !" << endl;
	}
	cout << endl << "Return to menu";

}


void Générer_fichier_heures_es_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour, float orientation, float inclinaison)
{
	int heure;
	int minute;
	int seconde;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;
	ofstream fichier2("Radiación_global_según_hora.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{



		heure = 1;
		minute = 0;
		seconde = 0;
		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		fichier2 << "hora, radiación directa (W/m^2), radiación difusa(W/m^2), radiación global(W/m^2)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);



			inclinaisonradian = Deg2rad(inclinaison);
			rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolairedirect = 0;
				rayonnementsolairediffus = 0;
			}


			azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);

			orientationradian = Deg2rad(orientation);
			coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

			rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);

			fichier2 << heure << "," << rayonnementsolairedirect << "," << rayonnementsolairediffus << "," << rayonnementsolaireglobal << endl;

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error en la apertura !" << endl;
	}
	cout << endl << "Volver al menu";

}


void Calculer_rayonnement_global_fr_or_incli(void)
{
	//Declaration de toutes les variables.
	int choix;//Choix, que faire à la fin de la fonction


	int d;//variables pour rentrer tous les paramètres
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;//variables pour rentrer tous les paramètres

	float longituderadian;//Variables servant à stocker les paramètres calculés par les fonctions
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	int meilleurorientation;
	int meilleurinclinaison;
	cout << endl;
	cout << "Choisissez votre environnement" << endl;
	cout << "1. Environnement industriel" << endl;
	cout << "2. Environnement urbain" << endl;
	cout << "3. Environnement rural" << endl;
	cout << "4. Environnement montagnard" << endl;
	cin >> environnement;
	cout << endl << "Entrez un r" << "\x82" << "el, la temp\x82rature en degr\x82s celsius et un r" << "\x82" << "el, le taux moyen d'humidit\x82 relative" << endl;
	cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
	cout << endl;
	cout << "Comment voulez vous rentrer les donn" << "\x82" << "es angulaires?" << endl;
	cout << " 1- En degr\x82s d" << "\x82" << "cimaux" << endl;
	cout << " autre entier- En degr\x82s sexag\x82simaux" << endl;//Choix donné à l'utilisateur pour les coordonnées sphériques
	cin >> d;
	if (d == 1)
	{
		cout << endl << "Entrez la latitude en degr\x82s d" << "\x82" << "cimaux, la longitude en degr\x82s d" << "\x82" << "cimaux ainsi que l'altitude ( 1 entier) en m\x8Atres" << endl;
		cin >> latitudedegré >> longitudedegré >> altitude;
	}
	else
	{
		cout << endl << "Entrez la latitude en degr\x82s sexag\x82simaux, la longitude en degr\x82s sexag\x82simaux ainsi que l'altitude (1 entier) en m\x8Atres" << endl;
		cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
		latitudedegré = Sex2deg(la1, la2, la3);
		longitudedegré = Sex2deg(lo1, lo2, lo3);
	}

	cout << endl << "Entrez une date : ann" << "\x82" << "e, mois, jour" << endl;
	cin >> année >> mois >> jour;
	cout << endl << "Entrez heure, minute, seconde" << endl;
	cin >> heure >> minute >> seconde;
	orientation = -179.0;
	inclinaison = 0.0;

	jièmejour = Mois_jourtojour(jour, mois, année);//Déterminer le jour de l'année en fonction de l'année, du mois et du jour
	nombrejourannée = Bissect(année);//Déterminer le nombre de jours dans l'année si année bissextile ou non
	vitesseangulaire = Vitesse_angulaire(nombrejourannée);
	vitesseangulaireradian = Deg2rad(vitesseangulaire);//conversion en radians
	déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

	heurelégale = hhmmss2dec(heure, minute, seconde);//Conversion en heure légale décimale

	longituderadian = Deg2rad(longitudedegré);//conversion en radians
	latituderadian = Deg2rad(latitudedegré);//conversion en radians
	//Calcul du temps solaire et de tous les paramètres en découlant
	tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
	anglehoraire = Angle_horaire(tempssolaire);
	anglehoraireradian = Deg2rad(anglehoraire);//conversion en radians
	hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);//Calcul de la hauteur solaire grâce
	// à tous les paramètres précédents

	//Calcul de la masse d'air optique relative
	pressionatmo = pression_atm(altitude);
	masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);
	//Calcul du facteur trouble de Linke
	pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
	pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
	facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

	epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
	energiesolaire = Ener_sol(jièmejour, nombrejourannée);
	//Calcul du rayonnement solaire direct grâce à tous les paramètres calculés précedemment
	rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);
	if (hauteursoleilradian < 0)
	{
		rayonnementsolairedirect = 0;
		rayonnementsolairediffus = 0;
		cout << "Il fait nuit." << endl;
	}
	else
	{
		rayonnementsolaireglobal = 0.0;
		while (orientation <= 180)
		{
			while (inclinaison <= 90)
			{
				inclinaisonradian = Deg2rad(inclinaison);
				rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

				azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
				orientationradian = Deg2rad(orientation);
				coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

				if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)
				{
					rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
					meilleurorientation = orientation;
					meilleurinclinaison = inclinaison;
				}
				inclinaison = inclinaison + 1.0;
			}
			inclinaison = 0;
			orientation = orientation + 1.0;
		}

		if (meilleurorientation < 0)
		{

			cout << "Le rayonnement maximal est de " << rayonnementsolaireglobal << " pour une orientation de " << endl;
			cout << -meilleurorientation << " degr\x82s vers l'Est, et une inclinaison de  " << meilleurinclinaison << " degr\x82s." << endl;
		}
		else
		{
			cout << "Le rayonnement maximal est de " << rayonnementsolaireglobal << endl;
			cout << "pour une orientation de " << meilleurorientation << " degr\x82s vers l'Ouest, et une inclinaison de " << meilleurinclinaison << " degr\x82s." << endl;
		}

	}

	Générer_fichier_jours_fr_par_or_incli(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde);//Génère également un fichier automatiquement en fonction du jour
	Générer_fichier_heures_fr_par_or_incli(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour);//Génère également un fichier automatiquement en fonction de l'heure
	//Les fichiers sont générés avec les paramètres rentrés précedemment
	menu_francais();
}

void Générer_fichier_jours_fr_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde)
{
	float orientation;
	float inclinaison;
	int meilleurorientation;
	int meilleurinclinaison;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("rayonnement_global_maximal_en_fonction_du_jour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = 1;

		nombrejourannée = Bissect(année);
		fichier << "jour,Rayonnement solaire global maximal(W/m^2),orientation(degrés),inclinaison(degrés)" << endl;
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);

			if (hauteursoleilradian < 0)//Si la hauteur solaire est négative il fait nuit, on ne peut pas trouver l'orientation et inclinaison optimale.
			{
				rayonnementsolaireglobal = 0.0;
				fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;
			}
			else
			{
				orientation = -179.0;//Sinon on initialise l'orientation le plus à l'est possible
				inclinaison = 0.0;//L'inclinaison est la plus basse possible
				rayonnementsolaireglobal = 0.0;
				while (orientation <= 180)
				{
					while (inclinaison <= 90)
					{
						inclinaisonradian = Deg2rad(inclinaison);
						rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

						azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
						orientationradian = Deg2rad(orientation);
						coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

						if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)//Si le rayonnement est plus fort que celui précedemment calculé, alors 
						{
							rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);//On remplace l'ancienne valeur par la nouvelle
							meilleurorientation = orientation;//On sauvegarde le renseignement de l'orientation et inclinaison correspondante
							meilleurinclinaison = inclinaison;
						}
						inclinaison = inclinaison + 1.0;
					}
					inclinaison = 0;
					orientation = orientation + 1.0;
				}
				fichier << jièmejour << "," << rayonnementsolaireglobal << "," << meilleurorientation << "," << meilleurinclinaison << endl;


			}

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Erreur à l'ouverture !" << endl;
	}
	cout << endl << "retour au menu";
}

void Générer_fichier_heures_fr_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour)
{

	int heure;
	int minute;
	int seconde;
	float orientation;
	float inclinaison;
	int meilleurorientation;
	int meilleurinclinaison;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier2("rayonnement_global_maximal_en_fonction_de_lheure.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		heure = 1;
		minute = 0;
		seconde = 0;
		fichier2 << "heure,Rayonnement solaire global maximal(W/m^2),orientation(degrés),inclinaison(degrés)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolaireglobal = 0.0;
				fichier2 << heure << "," << rayonnementsolaireglobal << "," << endl;
			}
			else
			{
				orientation = -179.0;
				inclinaison = 0.0;
				rayonnementsolaireglobal = 0.0;
				while (orientation <= 180)
				{
					while (inclinaison <= 90)
					{
						inclinaisonradian = Deg2rad(inclinaison);
						rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

						azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
						orientationradian = Deg2rad(orientation);
						coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

						if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)
						{
							rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
							meilleurorientation = orientation;
							meilleurinclinaison = inclinaison;
						}
						inclinaison = inclinaison + 1.0;
					}
					inclinaison = 0;
					orientation = orientation + 1.0;
				}
				fichier2 << heure << "," << rayonnementsolaireglobal << "," << meilleurorientation << "," << meilleurinclinaison << endl;


			}

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Erreur à l'ouverture !" << endl;
	}
	cout << endl << "retour au menu";
}









void Calculer_rayonnement_global_en_or_incli(void)
{
	//Declaration de toutes les variables.
	int choix;//Choix, que faire à la fin de la fonction


	int d;//variables pour rentrer tous les paramètres
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;//variables pour rentrer tous les paramètres

	float longituderadian;//Variables servant à stocker les paramètres calculés par les fonctions
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	int meilleurorientation;
	int meilleurinclinaison;
	cout << endl;
	cout << "Select an environment" << endl;
	cout << "1. Industrial environment" << endl;
	cout << "2. Urban environment" << endl;
	cout << "3. Rural environment" << endl;
	cout << "4. Mountain environment" << endl;
	cin >> environnement;
	cout << endl << "Enter 1 real number, the temperature in celsius degrees and one real number, the average relative humidity" << endl;
	cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
	cout << "How would you like to input the angular data?" << endl;
	cout << " 1- In decimal degrees" << endl;
	cout << " other integer- In sexagesimal degrees" << endl;
	cin >> d;
	if (d == 1)
	{
		cout << endl << "Enter the latitude in decimal degrees, the longitude in decimal degrees and the height ( 1 integer) in meters" << endl;
		cin >> latitudedegré >> longitudedegré >> altitude;
	}
	else
	{
		cout << endl << "Enter the latitude in sexagesimal degrees, the longitude in sexagesimal degrees and the height ( 1 integer) in meters" << endl;
		cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
		latitudedegré = Sex2deg(la1, la2, la3);
		longitudedegré = Sex2deg(lo1, lo2, lo3);
	}

	cout << endl << "Enter a date : year, month, day" << endl;
	cin >> année >> mois >> jour;
	cout << endl << "Enter hour, minute, second" << endl;
	cin >> heure >> minute >> seconde;
	orientation = -179.0;
	inclinaison = 0.0;

	jièmejour = Mois_jourtojour(jour, mois, année);//Déterminer le jour de l'année en fonction de l'année, du mois et du jour
	nombrejourannée = Bissect(année);//Déterminer le nombre de jours dans l'année si année bissextile ou non
	vitesseangulaire = Vitesse_angulaire(nombrejourannée);
	vitesseangulaireradian = Deg2rad(vitesseangulaire);//conversion en radians
	déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

	heurelégale = hhmmss2dec(heure, minute, seconde);//Conversion en heure légale décimale

	longituderadian = Deg2rad(longitudedegré);//conversion en radians
	latituderadian = Deg2rad(latitudedegré);//conversion en radians
	//Calcul du temps solaire et de tous les paramètres en découlant
	tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
	anglehoraire = Angle_horaire(tempssolaire);
	anglehoraireradian = Deg2rad(anglehoraire);//conversion en radians
	hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);//Calcul de la hauteur solaire grâce
	// à tous les paramètres précédents

	//Calcul de la masse d'air optique relative
	pressionatmo = pression_atm(altitude);
	masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);
	//Calcul du facteur trouble de Linke
	pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
	pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
	facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

	epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
	energiesolaire = Ener_sol(jièmejour, nombrejourannée);
	//Calcul du rayonnement solaire direct grâce à tous les paramètres calculés précedemment
	rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);
	if (hauteursoleilradian < 0)
	{
		rayonnementsolairedirect = 0;
		rayonnementsolairediffus = 0;
		cout << "It is night time" << endl;
	}
	else
	{
		rayonnementsolaireglobal = 0.0;
		while (orientation <= 180)
		{
			while (inclinaison <= 90)
			{
				inclinaisonradian = Deg2rad(inclinaison);
				rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

				azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
				orientationradian = Deg2rad(orientation);
				coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

				if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)
				{
					rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
					meilleurorientation = orientation;
					meilleurinclinaison = inclinaison;
				}
				inclinaison = inclinaison + 1.0;
			}
			inclinaison = 0;
			orientation = orientation + 1.0;
		}

		if (meilleurorientation < 0)
		{

			cout << "The maximum global radiation is" << rayonnementsolaireglobal << "Watt per meter square" << endl;
			cout << "for an orientation of " << -meilleurorientation << " degrees East, and an angle of " << meilleurinclinaison << " degrees." << endl;
		}
		else
		{
			cout << "The maximum global radiation is" << rayonnementsolaireglobal << "Watt per meter square" << endl;
			cout << "for an orientation of " << meilleurorientation << " degrees west, and an angle of " << meilleurinclinaison << " degrees." << endl;
		}

	}

	Générer_fichier_jours_en_par_or_incli(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde);//Génère également un fichier automatiquement en fonction du jour
	Générer_fichier_heures_en_par_or_incli(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour);//Génère également un fichier automatiquement en fonction de l'heure
	//Les fichiers sont générés avec les paramètres rentrés précedemment
	menu_english();
}

void Générer_fichier_jours_en_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde)
{
	float orientation;
	float inclinaison;
	int meilleurorientation;
	int meilleurinclinaison;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("maximum_global_radiation_depending_on_day.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = 1;

		nombrejourannée = Bissect(année);
		fichier << "day,maximum global radiation(W/m^2),orientation(degrees),angle(degrees)" << endl;
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);

			if (hauteursoleilradian < 0)//Si la hauteur solaire est négative il fait nuit, on ne peut pas trouver l'orientation et inclinaison optimale.
			{
				rayonnementsolaireglobal = 0.0;
				fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;
			}
			else
			{
				orientation = -179.0;//Sinon on initialise l'orientation le plus à l'est possible
				inclinaison = 0.0;//L'inclinaison est la plus basse possible
				rayonnementsolaireglobal = 0.0;
				while (orientation <= 180)
				{
					while (inclinaison <= 90)
					{
						inclinaisonradian = Deg2rad(inclinaison);
						rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

						azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
						orientationradian = Deg2rad(orientation);
						coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

						if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)//Si le rayonnement est plus fort que celui précedemment calculé, alors 
						{
							rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);//On remplace l'ancienne valeur par la nouvelle
							meilleurorientation = orientation;//On sauvegarde le renseignement de l'orientation et inclinaison correspondante
							meilleurinclinaison = inclinaison;
						}
						inclinaison = inclinaison + 1.0;
					}
					inclinaison = 0;
					orientation = orientation + 1.0;
				}
				fichier << jièmejour << "," << rayonnementsolaireglobal << "," << meilleurorientation << "," << meilleurinclinaison << endl;


			}

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error at the opening !" << endl;
	}

}

void Générer_fichier_heures_en_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour)
{

	int heure;
	int minute;
	int seconde;
	float orientation;
	float inclinaison;
	int meilleurorientation;
	int meilleurinclinaison;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier2("maximum_global_radiation_depending_on_hour.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		heure = 1;
		minute = 0;
		seconde = 0;
		fichier2 << "hour,maximum global radiation(W/m^2),orientation(degrees),angle(degrees)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolaireglobal = 0.0;
				fichier2 << heure << "," << rayonnementsolaireglobal << "," << endl;
			}
			else
			{
				orientation = -179.0;
				inclinaison = 0.0;
				rayonnementsolaireglobal = 0.0;
				while (orientation <= 180)
				{
					while (inclinaison <= 90)
					{
						inclinaisonradian = Deg2rad(inclinaison);
						rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

						azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
						orientationradian = Deg2rad(orientation);
						coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

						if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)
						{
							rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
							meilleurorientation = orientation;
							meilleurinclinaison = inclinaison;
						}
						inclinaison = inclinaison + 1.0;
					}
					inclinaison = 0;
					orientation = orientation + 1.0;
				}
				fichier2 << heure << "," << rayonnementsolaireglobal << "," << meilleurorientation << "," << meilleurinclinaison << endl;


			}

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "Error at the opening !" << endl;
	}
	cout << endl << "Return to menu";
}













void Calculer_rayonnement_global_es_or_incli(void)
{
	//Declaration de toutes les variables.
	int choix;//Choix, que faire à la fin de la fonction


	int d;//variables pour rentrer tous les paramètres
	int la1;
	int la2;
	int la3;
	int lo1;
	int lo2;
	int lo3;

	int année;
	int mois;
	int jour;
	int heure;
	int minute;
	int seconde;

	float temperaturedegréscelsius;
	float tauxmoyenhumiditérel;
	int environnement;

	float orientation;
	float inclinaison;

	float longitudedegré;
	int altitude;
	float latitudedegré;//variables pour rentrer tous les paramètres

	float longituderadian;//Variables servant à stocker les paramètres calculés par les fonctions
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	int meilleurorientation;
	int meilleurinclinaison;
	cout << endl;
	cout << "Elige un ambiante" << endl;
	cout << "1.   Ambiante industrial" << endl;
	cout << "2.   Ambiante urbano" << endl;
	cout << "3.   Ambiante rural" << endl;
	cout << "4.   Ambiante monta\xA4osa" << endl;
	cin >> environnement;
	cout << endl << "Entrar 1 n\xA3mero real, la temperatura en grados celsius y un n\xA3mero real, la humedad relativa media" << endl;
	cin >> temperaturedegréscelsius >> tauxmoyenhumiditérel;
	cout << "C" << "\xA2" << "mo quer\x82is entrar los datos angulares?" << endl;
	cout << " 1- En grados decimales" << endl;
	cout << " Otro entero- En grados sexagesimales" << endl;
	cin >> d;
	if (d == 1)
	{
		cout << endl << "Entrar la latitud en grados decimales, la longitud en grados decimales y la altitud ( 1 entero) en metros" << endl;
		cin >> latitudedegré >> longitudedegré >> altitude;
	}
	else
	{
		cout << endl << "Entrar la latitud en grados sexagesimales, la longitud en grados sexagesimales y la altitud ( 1 entero) en metros" << endl;
		cin >> la1 >> la2 >> la3 >> lo1 >> lo2 >> lo3 >> altitude;
		latitudedegré = Sex2deg(la1, la2, la3);
		longitudedegré = Sex2deg(lo1, lo2, lo3);
	}

	cout << endl << "Entrar una fecha : a\xA4o, mes, d" << "\xA1" << "a" << endl;
	cin >> année >> mois >> jour;
	cout << endl << "Entrar hora, minuto, segunda" << endl;
	cin >> heure >> minute >> seconde;
	orientation = -179.0;
	inclinaison = 0.0;

	jièmejour = Mois_jourtojour(jour, mois, année);//Déterminer le jour de l'année en fonction de l'année, du mois et du jour
	nombrejourannée = Bissect(année);//Déterminer le nombre de jours dans l'année si année bissextile ou non
	vitesseangulaire = Vitesse_angulaire(nombrejourannée);
	vitesseangulaireradian = Deg2rad(vitesseangulaire);//conversion en radians
	déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);

	heurelégale = hhmmss2dec(heure, minute, seconde);//Conversion en heure légale décimale

	longituderadian = Deg2rad(longitudedegré);//conversion en radians
	latituderadian = Deg2rad(latitudedegré);//conversion en radians
	//Calcul du temps solaire et de tous les paramètres en découlant
	tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
	anglehoraire = Angle_horaire(tempssolaire);
	anglehoraireradian = Deg2rad(anglehoraire);//conversion en radians
	hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);//Calcul de la hauteur solaire grâce
	// à tous les paramètres précédents

	//Calcul de la masse d'air optique relative
	pressionatmo = pression_atm(altitude);
	masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);
	//Calcul du facteur trouble de Linke
	pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
	pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
	facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

	epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
	energiesolaire = Ener_sol(jièmejour, nombrejourannée);
	//Calcul du rayonnement solaire direct grâce à tous les paramètres calculés précedemment
	rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);
	if (hauteursoleilradian < 0)
	{
		rayonnementsolairedirect = 0;
		rayonnementsolairediffus = 0;
		cout << "Es de noche." << endl;
	}
	else
	{
		rayonnementsolaireglobal = 0.0;
		while (orientation <= 180)
		{
			while (inclinaison <= 90)
			{
				inclinaisonradian = Deg2rad(inclinaison);
				rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

				azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
				orientationradian = Deg2rad(orientation);
				coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

				if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)
				{
					rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
					meilleurorientation = orientation;
					meilleurinclinaison = inclinaison;
				}
				inclinaison = inclinaison + 1.0;
			}
			inclinaison = 0;
			orientation = orientation + 1.0;
		}

		if (meilleurorientation < 0)
		{

			cout << "La maxima radiaci\xA2n es de " << rayonnementsolaireglobal << " vatios por metro cuadrado" << endl;
			cout << "para una orientaci\xA2n de " << -meilleurorientation << " grados hacia el este y una inclinaci\xA2n de " << meilleurinclinaison << " grados." << endl;
		}
		else
		{
			cout << "La maxima radiaci\xA2n es de  " << rayonnementsolaireglobal << " vatios por metro cuadrado" << endl;
			cout << "para una orientaci\xA2n de " << meilleurorientation << " grados hacia el oeste y una inclinaci\xA2n de " << meilleurinclinaison << " grados." << endl;
		}

	}

	Générer_fichier_jours_es_par_or_incli(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, heure, minute, seconde);//Génère également un fichier automatiquement en fonction du jour
	Générer_fichier_heures_es_par_or_incli(environnement, temperaturedegréscelsius, tauxmoyenhumiditérel, latitudedegré, longitudedegré, altitude, année, mois, jour);//Génère également un fichier automatiquement en fonction de l'heure
	//Les fichiers sont générés avec les paramètres rentrés précedemment
	menu_espanol();
}

void Générer_fichier_jours_es_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde)
{
	float orientation;
	float inclinaison;
	int meilleurorientation;
	int meilleurinclinaison;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier("maxima_radiacion_segun_el_dia.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = 1;

		nombrejourannée = Bissect(année);
		fichier << "día, radiación solar global máxima (W / m ^ 2), orientación (grados), inclinación (grados)" << endl;
		while (jièmejour <= nombrejourannée)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);

			if (hauteursoleilradian < 0)//Si la hauteur solaire est négative il fait nuit, on ne peut pas trouver l'orientation et inclinaison optimale.
			{
				rayonnementsolaireglobal = 0.0;
				fichier << jièmejour << "," << rayonnementsolaireglobal << "," << endl;
			}
			else
			{
				orientation = -179.0;//Sinon on initialise l'orientation le plus à l'est possible
				inclinaison = 0.0;//L'inclinaison est la plus basse possible
				rayonnementsolaireglobal = 0.0;
				while (orientation <= 180)
				{
					while (inclinaison <= 90)
					{
						inclinaisonradian = Deg2rad(inclinaison);
						rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

						azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
						orientationradian = Deg2rad(orientation);
						coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

						if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)//Si le rayonnement est plus fort que celui précedemment calculé, alors 
						{
							rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);//On remplace l'ancienne valeur par la nouvelle
							meilleurorientation = orientation;//On sauvegarde le renseignement de l'orientation et inclinaison correspondante
							meilleurinclinaison = inclinaison;
						}
						inclinaison = inclinaison + 1.0;
					}
					inclinaison = 0;
					orientation = orientation + 1.0;
				}
				fichier << jièmejour << "," << rayonnementsolaireglobal << "," << meilleurorientation << "," << meilleurinclinaison << endl;


			}

			jièmejour = jièmejour + 1;
		}
		fichier.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "error al abrir !" << endl;
	}

}

void Générer_fichier_heures_es_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour)
{

	int heure;
	int minute;
	int seconde;
	float orientation;
	float inclinaison;
	int meilleurorientation;
	int meilleurinclinaison;

	float longituderadian;
	float latituderadian;
	float anglehoraireradian;
	float vitesseangulaireradian;
	float hauteursoleilradian;
	float inclinaisonradian;
	float azimutradian;
	float orientationradian;

	float pressionatmo;
	int jièmejour;
	int nombrejourannée;
	float vitesseangulaire;
	float déclinaison;
	float déclinaisonradian;
	float heurelégale;
	float tempssolaire;
	float anglehoraire;
	float hauteursoleil;
	float masseairoptiquerelative;

	double pressionpartiellevapeursaturante;
	float pressionpartiellevapeureau;
	float facteurtroublelinke;

	float epaisseuroptiquerayleigh;
	float energiesolaire;
	float rayonnementsolairedirect;



	float rayonnementsolairediffus;



	float azimut;
	float coeffincidence;

	float rayonnementsolaireglobal;

	//Actionne la fonction reliée
	ofstream fichier2("maxima_radiacion_segun_el_hora.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier

	if (fichier2)  // si l'ouverture a réussi
	{
		// instructions


		jièmejour = Mois_jourtojour(jour, mois, année);

		nombrejourannée = Bissect(année);
		heure = 1;
		minute = 0;
		seconde = 0;
		fichier2 << "hora, radiación solar global máxima (W / m ^ 2), orientación (grados), inclinación (grados)" << endl;
		while (heure <= 24)
		{


			pressionatmo = pression_atm(altitude);
			vitesseangulaire = Vitesse_angulaire(nombrejourannée);
			vitesseangulaireradian = Deg2rad(vitesseangulaire);
			déclinaisonradian = Declinaison(vitesseangulaireradian, jièmejour);
			heurelégale = hhmmss2dec(heure, minute, seconde);

			longituderadian = Deg2rad(longitudedegré);
			latituderadian = Deg2rad(latitudedegré);


			tempssolaire = Temps_solaire(année, jièmejour, heurelégale, longituderadian);
			anglehoraire = Angle_horaire(tempssolaire);
			anglehoraireradian = Deg2rad(anglehoraire);
			hauteursoleilradian = Hauteur_soleil(anglehoraireradian, latituderadian, déclinaisonradian);


			masseairoptiquerelative = Masse_air_optique(hauteursoleilradian, pressionatmo);

			pressionpartiellevapeursaturante = Pression_partielle_vap_sat(temperaturedegréscelsius);
			pressionpartiellevapeureau = Pression_partielle_vap_eau(tauxmoyenhumiditérel, pressionpartiellevapeursaturante);
			facteurtroublelinke = Facteur_trouble(environnement, pressionpartiellevapeureau);

			epaisseuroptiquerayleigh = Epaisseur_optique(masseairoptiquerelative);
			energiesolaire = Ener_sol(jièmejour, nombrejourannée);
			rayonnementsolairedirect = rayonnement_solaire_direct(energiesolaire, epaisseuroptiquerayleigh, masseairoptiquerelative, facteurtroublelinke);

			if (hauteursoleilradian < 0)
			{
				rayonnementsolaireglobal = 0.0;
				fichier2 << heure << "," << rayonnementsolaireglobal << "," << endl;
			}
			else
			{
				orientation = -179.0;
				inclinaison = 0.0;
				rayonnementsolaireglobal = 0.0;
				while (orientation <= 180)
				{
					while (inclinaison <= 90)
					{
						inclinaisonradian = Deg2rad(inclinaison);
						rayonnementsolairediffus = rayonnement_solaire_diffus(hauteursoleilradian, inclinaisonradian);

						azimutradian = Azimut(déclinaisonradian, anglehoraireradian, hauteursoleilradian);
						orientationradian = Deg2rad(orientation);
						coeffincidence = coefficient_incidence(hauteursoleilradian, orientationradian, azimutradian, inclinaisonradian);

						if (Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus) > rayonnementsolaireglobal)
						{
							rayonnementsolaireglobal = Rayonnement_solaire_global(rayonnementsolairedirect, coeffincidence, rayonnementsolairediffus);
							meilleurorientation = orientation;
							meilleurinclinaison = inclinaison;
						}
						inclinaison = inclinaison + 1.0;
					}
					inclinaison = 0;
					orientation = orientation + 1.0;
				}
				fichier2 << heure << "," << rayonnementsolaireglobal << "," << meilleurorientation << "," << meilleurinclinaison << endl;


			}

			heure = heure + 1;
		}
		fichier2.close();  // on referme le fichier
	}
	else  // sinon
	{
		cerr << "error al abrir !" << endl;
	}
	cout << endl << "volver al menu";
}