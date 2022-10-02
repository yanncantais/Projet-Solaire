//Jessie Bai
//JD Marion
//Quentin Sagnol


//R: Calcule et renvoie le nombre de jours
//E : L’année 1 entier
//S : 1 entier
int Bissect(int année);

//R: Calcule et renvoie le rayonnement solaire global
//E : I, Ci, D 3 réels
//S : G 1 réel
float Rayonnement_solaire_global(float i, float ci, float d);

//R: Calcule et renvoie la vitesse angulaire
//E : Le nombre de jours 1 entier
//S : 1 réel
float Vitesse_angulaire(int nbj);




//Anais Battut
//Leo Porte
//Jules Genet


//R:Conversion des heures hh mm ss en heures décimales
//E: heure mnutes et secondes
//S: L'heure décimale
float hhmmss2dec(int hh, int mm, int ss);

//R:Convertit une heure décimale en heure hh mm ss
//E: heure décimale
//S: Un tableau contenant l'heure la minute et la seconde
//int tableau[3] dec2hhmmss(float);

//R: Calcule et renvoie l’energie solaire recue
//E : le jour et le nombre de jours 2 entiers
//S : 1 réel
float Ener_sol(int j, int nbj);

//R: Calcule le rendement du capteur solaire
//E: Rayonnement solaire global
//S: Rendement du capteur solaire
float Rendement(float);
//Proposer d'abord comme approximer au mieux le rendement du panneau à l'aide du dossier


//Valentine Gilles
//Hugo Guillot
//Albi Shahu


//R: Calcule et renvoie l’azimut
//E: la déclinaison, la hauteur du soleil, l’angle horaire 3 réels
//S: 1 réel
float Azimut(float dec, float ah, float h);

//R: Calcule et renvoie le coefficient d’incidence
//E : la hauteur du soleil, l’inclinaison i, l’azimut a et l’orientation o 4 réels
//S : 1 réel
float coefficient_incidence(float h, float o, float a, float i);

//R: Calcule et renvoie le rayonnement solaire diffus
//E: la hauteur du soleil et l’inclinaison deux réels
//S: 1 réel
float rayonnement_solaire_diffus(float h, float i);




//Antoine Faurie
//Gael Riou
//Louis Reynaud


//R: Calcule et renvoie le temps solaire
//E: l’heure légale, muj et tetaj 3 réels
//S: 1 réel
double Temps_solaire(int annee, int jour, float hlegal, float longitudeRad);

//R: Calcule et renvoie le rayonnement solaire direct
//E: L’Energie solaire, l’épaisseur optique de rayleigh, le facteur de trouble de Linke, la masse d’air optique relative, 4 réels
//S: 1 réel
float rayonnement_solaire_direct(float esol, float er, float m, float tl);

//R: convertit des degrés en degrés sexagésimaux
//E: Angle en degrés
//S: Angle en degrés sexagesimaux
struct degsex {
	float deg;
	float min;
	float sec;
};
degsex Deg2sex(float degdec);

//A faire par gael et antoine
//R: Convertit mois + jour en jour
//E: 3 entiers, l'année, le mois, le jour
//S: Le jour de l'année correspondant
int Mois_jourtojour(int j, int m, int a);
//A faire par gael et antoine

//Pierre Corron
//Charles Dupont
//Mathis Magane


//R: Calcule et renvoie la hauteur solaire
//E: L’angle horaire, la latitude, la déclinaison 3 réels
//S: 1 réel
float Hauteur_soleil(float ah, float lat, float dec);

//R: Calcule et renvoie l’angle horaire
//E: Le temps solaire 1 réel
//S: 1 réel
float Angle_horaire(float tsv);

//R: Calcule et renvoie la déclinaison																			  
//E: Le jour 1 entier et la vitesse angulaire 1 réel																							  
//S: 1 réel																	
double Declinaison(float va, int j);

//R: convertit des degrés en radians
//E: L'angle en degrés
//S: L'angle en radians
float Deg2rad(float);



//Maxime Boichon																											 
//Jean Chaput																											 
//Dany Nguyen


//R: Calcule et renvoie la pression partielle de vapeur d’eau																											
//E: le taux moyen d’humidité relative et la pression de vapeurs saturantes 2 réels																											 
//S: 1 réel																																																						  
double Pression_partielle_vap_eau(float hr, double pvs);

//R: Calcule et renvoie la pression partielle de vapeurs saturantes																													  
//E: T la temperature en degrés celsius 1 réel																													  
//S: 1 réel																											
double Pression_partielle_vap_sat(float t);

//R: Calcule et renvoie l’épaisseur optique de Rayleigh																																			  
//E: la masse d’air optique 1 réel																																			 
//S: 1 réel
double Epaisseur_optique(float m);

//R: convertit des degrés sexagesimaux en degrés
//E: nombre de minutes d'angle
//S: Equivalent en degrés
float Sex2deg(int deg, int min, int sec);



//Yannis Guironnet																																									 					   
//Lilian Lamouille																																									  
//Claude Chambery


//R: Calcule et renvoie la pression atmosphérique																																									 
//E: altitude en metres 1 entier																																									 
//S: 1 réel																																																					 
float pression_atm(int z);



//R: Calcule et renvoie le facteur de trouble de Linke																																											
//E: la pression partielle de vapeur d’eau et le facteur de trouble atmospherique 2 réels																																												 
//S: 1 réel																																								 																																											   
float Facteur_trouble(int c, float pv);


//R: Calcule et renvoie la masse d’air optique																																															   
//E: 2 réels la pression atmosphérique et la hauteur solaire																																														   
//S: 1 réel																																														   																																																		
float Masse_air_optique(float h, float patm);

//R: convertit des radians en degrés
//E: L'angle en radians
//S: L'angle en degrés
float Rad2deg(float);





void langue(void);

void menu_francais(void);
void menu_english(void);
void menu_espanol(void);

void Calculer_rayonnement_global_fr(void);
void Calculer_rayonnement_global_en(void);
void Calculer_rayonnement_global_es(void);

void Générer_fichier_heures_fr(void);
void Générer_fichier_heures_en(void);
void Générer_fichier_heures_es(void);

void Générer_fichier_jours_fr(void);
void Générer_fichier_jours_en(void);
void Générer_fichier_jours_es(void);



void Générer_fichier_jours_fr_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde, float orientation, float inclinaison);
void Générer_fichier_jours_en_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde, float orientation, float inclinaison);
void Générer_fichier_jours_es_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde, float orientation, float inclinaison);

void Générer_fichier_heures_fr_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour, float orientation, float inclinaison);
void Générer_fichier_heures_en_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour, float orientation, float inclinaison);
void Générer_fichier_heures_es_par(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour, float orientation, float inclinaison);


void Calculer_rayonnement_global_fr_or_incli(void);
void Générer_fichier_jours_fr_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde);
void Générer_fichier_heures_fr_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année,int mois,int jour);

void Calculer_rayonnement_global_en_or_incli(void);
void Générer_fichier_jours_en_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde);
void Générer_fichier_heures_en_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour);

void Calculer_rayonnement_global_es_or_incli(void);
void Générer_fichier_jours_es_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int heure, int minute, int seconde);
void Générer_fichier_heures_es_par_or_incli(int environnement, float temperaturedegréscelsius, float tauxmoyenhumiditérel, float latitudedegré, float longitudedegré, int altitude, int année, int mois, int jour);