#include"balle.h"
#include<cmath>
#define PI 3.14159265359
	balle::balle()
	{
		setX(400);
		setY(300);
		setW(10);
		setH(10);
		setK(2);
		setVx(3);
		setVy(0);
		_angle = 0;
		_vitesse = 5;
	}
	balle::~balle()
	{

	}
	void balle::respawn()
	{
		setX(400);
		setY(300);
	}
	bool balle::colision(Rect rect)
	{
	   if((rect.x >= _rect.x + _rect.w)     // trop à droite
		|| (rect.x + rect.w <= _rect.x)		// trop à gauche
		|| (rect.y >= _rect.y + _rect.h)	// trop en bas
		|| (rect.y + rect.h <= _rect.y))	// trop en haut
			  return false;
	   else
			  return true;
	}
	void balle::move()
	{
		_vy = (_vitesse * _k)*sin((float )_angle*(PI/180));
		_vx = (_vitesse * _k)*cos((float )_angle*(PI/180));
		_rect.x += _vx;
		_rect.y += _vy;
	}

	void balle::changeAngle(Rect palette)
	{
		int echelle;
		echelle = 60/palette.h;
		//Inverse sense
		_angle = ((_angle - 180)-90)+90;
		while(_angle < 0)
		{
			_angle+=360;
		}
		if(_vx > 0)
			_angle+=((palette.h/2 + palette.y)-(_rect.h/2 + _rect.y))*echelle;
		else
			_angle-=((palette.h/2 + palette.y)-(_rect.h/2 + _rect.y))*echelle;
	}

	void balle::murRebond()
	{
		_angle = ((_angle - 270)-180)+180;
		while(_angle < 0)
		{
			_angle+=360;
		}
	}
	//get
	int balle::getVx()
	{
		return _vx;
	}
	int balle::getVy()
	{
		return _vy;
	}
	int balle::getK()
	{
		return _k;
	}
	int balle::getX()
	{
		return _rect.x;
	}
	int balle::getY()
	{
		return _rect.y;
	}
	int balle::getW()
	{
		return _rect.w;
	}
	int balle::getH()
	{
		return _rect.h;
	}
	Rect balle::getrect()
	{
		return _rect;
	}
	//set
	void balle::setVx(int vx)
	{
		_vx = vx;
	}
	void balle::setVy(int vy)
	{
		_vy = vy;
	}
	void balle::setK(int k)
	{
		_k = k;
	}
	void balle::setX(int x)
	{
		_rect.x = x;
	}
	void balle::setY(int y)
	{
		_rect.y = y;
	}
	void balle::setW(int w)
	{
		_rect.w = w;
	}
	void balle::setH(int h)
	{
		_rect.h = h;
	}


	//Joueur
	joueur::joueur(int number)
	{
		_rect.w = 10;
		_rect.h = 60;
		_rect.y = 275;
		if(number == 1)
		{
			_rect.x = 5;
		}
		else
		{
			_rect.x = 785;
		}
	}
	joueur::~joueur()
	{
	
	}
	void joueur::bougerHaut()
	{
		_rect.y -= 5;
	}
	void joueur::bougerBas()
	{
		_rect.y += 5;
	}
	void joueur::scoreGoal()
	{
		_score++;
	}
	//get
	int joueur::getVy()
	{
		return _vy;
	}
	int joueur::getX()
	{
		return _rect.x;
	}
	int joueur::getY()
	{
		return _rect.y;
	}
	int joueur::getW()
	{
		return _rect.w;
	}
	int joueur::getH()
	{
		return _rect.h;
	}
	int joueur::getScore()
	{
		return _score;
	}

	Rect joueur::getrect()
	{
		return _rect;
	}
	//set
	void joueur::setVy(int vy)
	{
	
	}
	void joueur::setY(int y)
	{
	
	}
	void joueur::setW(int w)
	{
	
	}
	void joueur::setH(int h)
	{
	
	}
	void joueur::setScore(int score)
	{
	
	}