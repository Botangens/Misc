/*Animation module.
* Contains Frame and Animation class descriptions.
* Depends on SFML graphics module, which in turn requires SFML window and system inclusion.
*/

//Dependencies:
#include <SFML/Graphics.hpp>
#include <fstream> // for file loading
#include <sstream> // -||-
#include <string>  // handles naming
#include <math.h>
#include <Timer.hpp> // handles time 
//#include <sqlite3.h> // for DB loading - disabled for now

/*=================================================================================================
* Struct Aniclock - wrapper structure for Perspective::Timer;
* Used to get elapsed time from Timer object;
* Stores playback start timel
* May require a better name;
*/

struct Aniclock
{
	Perspective::Duration stime = 0;
	Perspective::Timer * timer = nullptr;
};


/*=================================================================================================
* Class Frame - for handling individual frames; 
*contains information required to extract an individual frame of animation from a texture object;
*/

class Frame
{
public:
	int x;
	int y;
	int width;
	int height;

//Constructors
	Frame();
	Frame(int x1, int x2, int x3, int x4);
	~Frame();
//various functions
	void init(int x1, int x2, int x3, int x4); // allows to define the Frame attributes after the default constructor has been called 
	sf::IntRect getRect(); // extracts an SFML Rectangle object for use in texture object partitioning;
};

/*=================================================================================================
* Class Animation - allows to group and order Frames; 
* Contains a sequence of Frames, as well as playback information;
*/

class Animation
{
private:
public:
	Frame * frames;
	int length; // Maximum number of Frames;
	double FPS = 1; // Number of frames to be played per second - defines speed of playback; 
	std::string name; // Identificator;
	sf::Sprite * sprite; //Allows to use Animation object without initialising an Anisprite object;
	sf::Texture * texture; //Same; 

	//Constructors
	Animation();
	Animation(int n, std::string line, sf::Texture * t);
	Animation(int n, std::string line, sf::Texture * t, sf::Sprite * s);
	~Animation();

	//Various functions
	void init(int n, std::string line, sf::Texture * t, sf::Sprite * s);
	void init(int n, double f, std::string line, sf::Texture * t, sf::Sprite * s);//init functions for defining attributes after default constructor calls;
	void setfps(double f);//sets FPS parametre
	void addFrame(int x, int y, int width, int height); //adds Frame information to array; 
	int getLength(); // returns animation length

};

/*=================================================================================================
* Class Projection - a wrapper class for Animation array; 
* Contains information about the number of Animations;
* May require a better name;
*/

class Projection
{
public:
	int length;
	Animation * animations;

	//Constructors
	Projection();
	Projection(int n);
	~Projection();

	//Initialisation functions - may require more overloads;
	void init(int n);
};

/*=================================================================================================
* Class Anisprite - main class for animation playback handling;
* Contains a 2D array of Animations, as well as SFML Sprite and Texture objects for playback;
* May require a better name;
*/

class Anisprite
{
public:
	sf::Texture texture;
	sf::Sprite sprite;
	Aniclock _clock;
	char * picdata;
	Projection * projections;
	int projn;
	int cur_proj;
	int cur_anim;

	//Constructors:  
	Anisprite();
	Anisprite(int n, std::string source);
	~Anisprite();

	//Initialisation and loading functions:
	void init(int n, std::string source);
	void init(int n);
	void init_timer(Perspective::Timer * t);
	void loadFromFile(std::string data, std::string source);
	void loadFromMemory(char * mdata);
	void loadFromDB(int &len, char * data);

    //Playback functions:
	void setplayback(int n, int m);
	void setplayback(int n);
	void updateSprite(int n, int t);
	void updateSprite(int t);
	void loopUpdate();
	void setPosition(float x, float y);
	int switchProj(int n);
	int switchAnim(int n);
};