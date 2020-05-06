#ifndef WEAPON_H
#define WEAPON_H
enum weapons {
	AK47,
	USP_S
};


class Weapon {
	private:
		int damage;
		int reloadTime;
		/* behaviour */
		Texture2D texture;
		int texWidth, texHeight;
	public:
		Weapon(Texture2D _texture, int _texWidth, int _texHeight){
			texture   = _texture;
			texWidth  = _texWidth;
			texHeight = _texHeight;
		}
		
		//static void drawWeapons(

};
#endif
