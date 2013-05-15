//Position et largeure des objet
struct Rect
{
	int x;	//position x
	int y;	//position y
	int w;	//Largeure
	int h;	//Hauteur
};

//information a envoyer
struct sendInfo
{
	Rect joueur[2];
	Rect balle;
};

//Objet représentant la balle du jeu pong
class balle
{
public:
	balle();					//constructeur
	~balle();					//destructeur
	void respawn();				//Respawn la balle apres le but
	bool colision(Rect rect);	//Regarde la colision avec un autre objet
	//get
	int getVx();
	int getVy();
	int getK();
	int getX();
	int getY();
	int getW();
	int getH();
	Rect getrect();
	void move();
	//set
	void setVx(int vx);
	void setVy(int vy);
	void setK(int k);
	void setX(int x);
	void setY(int y);
	void setW(int w);
	void setH(int h);
private:
	Rect _rect;
	int _vx;
	int _vy;
	int _k;		//Multiplicateur de vitesse
};

//Object representant un joueur
class joueur
{
public:
	joueur(int number);
	~joueur();
	void bougerHaut();
	void bougerBas();
	void scoreGoal();
	//get
	int getVy();
	int getX();
	int getY();
	int getW();
	int getH();
	int getScore();
	Rect getrect();
	//set
	void setVy(int vy);
	void setX(int x);
	void setY(int y);
	void setW(int w);
	void setH(int h);
	void setScore(int score);
private:
	Rect _rect;
	int _vy;	//vitesse sur l'axe des y
	int _score;	//Score d'un joueur
};
