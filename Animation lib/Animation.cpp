#include "Animation.hpp"

//=====================================FRAME=================================
Frame::Frame()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

Frame::Frame(int x1, int x2, int x3, int x4)
{
	x = x1;
	y = x2;
	width = x3;
	height = x4;
}

Frame::~Frame()
{
}

void Frame::init(int x1, int x2, int x3, int x4)
{
	x = x1;
	y = x2;
	width = x3;
	height = x4;
}

sf::IntRect Frame::getRect()
{
	sf::IntRect rect(x, y, width, height);
	return rect;
}


//=====================================ANIMATION=================================
Animation::Animation()
{
}

Animation::Animation(int n, std::string line, sf::Texture * t)
{
	length = n;
	name = line;
	texture = t;
	frames = new Frame[length];
}

Animation::Animation(int n, std::string line, sf::Texture * t, sf::Sprite * s)
{
	length = n;
	name = line;
	texture = t;
	sprite = s;
	frames = new Frame[length];
}

Animation::~Animation()
{
	delete[] frames;
}


void Animation::init(int n, std::string line, sf::Texture * t, sf::Sprite * s)
{
	length = n;
	name = line;
	texture = t;
	sprite = s;
	frames = new Frame[length];
}

void Animation::init(int n, double f, std::string line, sf::Texture * t, sf::Sprite * s)
{
	length = n;
	name = line;
	texture = t;
	sprite = s;
	FPS = f;
	frames = new Frame[length];
}

void Animation::setfps(double f)
{
	FPS = f;
}

void Animation::addFrame(int x, int y, int width, int height)
{

	for (int i = 0; i<length; i++)
	{
		if (frames[i].height == 0 && frames[i].width == 0)
		{
			frames[i].x = x;
			frames[i].y = y;
			frames[i].width = width;
			frames[i].height = height;
			break;
		}
	}
}


int Animation::getLength()
{
	return length;
}
//=====================================PROJECTION=================================

Projection::Projection()
{
}

Projection::Projection(int n)
{
	length = n;
	animations = new Animation[length];
}

void Projection::init(int n)
{
	length = n;
	animations = new Animation[length];
}

Projection::~Projection()
{
	delete[] animations;
}

//=====================================ANISPRITE=================================
Anisprite::Anisprite()
{
	cur_proj = 0;
	cur_anim = 0;
	sprite.setTexture(texture);
}

Anisprite::Anisprite(int n, std::string source)
{
	cur_proj = 0;
	cur_anim = 0;
	projn = n;
	projections = new Projection[projn];
	texture.loadFromFile(source);
	sprite.setTexture(texture);
}

Anisprite::~Anisprite()
{
	delete[] projections;
}



void Anisprite::updateSprite(int n, int t)
{
	sprite.setTextureRect(projections[cur_proj].animations[n].frames[t].getRect());
}

void Anisprite::updateSprite(int t)
{
	sprite.setTextureRect(projections[cur_proj].animations[cur_anim].frames[t].getRect());
}

void Anisprite::loopUpdate()
{
	_clock.timer->Update();
	float ctime = float(_clock.timer->GetTime().asSec() - _clock.stime);
	int t = int(floor(ctime * projections[cur_proj].animations[cur_anim].FPS)) % projections[cur_proj].animations[cur_anim].length;
	updateSprite(t);
}

int Anisprite::switchProj(int n)
{
	if (n<projn && n>-1)
	{
		cur_proj = n;
		cur_anim = 0;
		return 1;
	}
	else return 0;
}

int Anisprite::switchAnim(int n)
{
	if (n<projections[cur_proj].length && n>-1)
	{
		cur_anim = n;
		return 1;
	}
	else return 0;
}

void Anisprite::init(int n, std::string source)
{
	cur_proj = 0;
	cur_anim = 0;
	projn = n;
	projections = new Projection[projn];
	texture.loadFromFile(source);
	sprite.setTexture(texture);
}

void Anisprite::init(int n)
{
	cur_proj = 0;
	cur_anim = 0;
	projn = n;
	projections = new Projection[projn];
	sprite.setTexture(texture);
}

void Anisprite::init_timer(Perspective::Timer * t)
{
	_clock.timer = t;
}

void Anisprite::setplayback(int n, int m)
{
	switchProj(n);
	switchAnim(m);
	_clock.stime = _clock.timer->Update();
}

void Anisprite::setplayback(int n)
{
	cur_anim = n;
	_clock.stime = _clock.timer->Update();
}

void Anisprite::loadFromFile(std::string data, std::string source)
{
	int n, m, k;
	double f;
	float x, y, width, height;
	std::fstream load(data.c_str());
	load >> n;
	this->init(n, source);
	for (int i = 0; i<n; i++)
	{
		load >> m;
		projections[i].init(m);
		for (int j = 0; j<m; j++)
		{
			load >> k;
			load >> f;
			projections[i].animations[j].init(k, f, source.c_str(), &texture, &sprite);
			for (int l = 0; l<k; l++)
			{
				load >> x >> y >> width >> height;
				projections[i].animations[j].addFrame(x, y, width, height);
			}
		}
	}

}

void Anisprite::loadFromMemory(char * mdata)
{
	std::stringstream load(mdata);
	int n, m, k;
	double f;
	float x, y, width, height;
	load >> n;
	this->init(n);
	for (int i = 0; i<n; i++)
	{
		load >> m;
		projections[i].init(m);
		for (int j = 0; j<m; j++)
		{
			load >> k;
			load >> f;
			projections[i].animations[j].init(k, f, "name", &texture, &sprite);
			for (int l = 0; l<k; l++)
			{
				load >> x >> y >> width >> height;
				projections[i].animations[j].addFrame(x, y, width, height);
			}
		}
	}
}

/*void Anisprite::loadFromDB(int &len, char * data)
{
int mlen;
char * mdata;
sqlite3 *db;
sqlite3_open("data.db", &db);
sqlite3_stmt * out;

char * sql = "SELECT mdata FROM blobs WHERE key = 1;";
sqlite3_prepare_v2(db, sql, -1, &out, 0);
sqlite3_step(out);
mlen = sqlite3_column_bytes(out, 0);
mdata = (char *)sqlite3_column_blob(out, 0);


sql = "SELECT data FROM blobs WHERE key = 1;";
sqlite3_prepare_v2(db, sql, -1, &out, 0);
sqlite3_step(out);
len = sqlite3_column_bytes(out, 0);
data = (char *)sqlite3_column_blob(out, 0);

loadFromMemory(mdata);
texture.loadFromMemory(data, len);

sqlite3_finalize(out);
delete mdata;
}*/



void Anisprite::setPosition(float x, float y)
{
	sprite.setPosition(x, y);
}


//=====================================END=================================
