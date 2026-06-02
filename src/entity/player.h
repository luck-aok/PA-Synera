#ifndef PLAYER_H
#define PLAYER_H
class Player{
private:
    int p_Hp = 100;
    int p_Gold = 10;
    int p_Level = 1;
    int p_PopulationCap = 3;
    int p_PopulationUsed = 0;
    //如果是PVP，还要有对敌方造成伤害，现在先不管
public:
    int Hp() const {return p_Hp;}
    int Gold() const{return p_Gold;}
    int Level() const{return p_Level;}
    int PopulationCap() const{return p_PopulationCap;}
    int PopulationUsed() const{return p_PopulationUsed;}
    void setHp(int hp){ p_Hp = hp;}
    void setGold(int Gold){p_Gold =Gold;}
    void changeGold(int changeValue){p_Gold +=changeValue;}//可以是负值
    void setLevel(int level){p_Level = level;}
    void setPopulationCap(int PopulationCap){p_PopulationCap = PopulationCap;}
    void changePopulationCap(int changeValue){p_PopulationCap += changeValue;}
    void setPopulationUsed(int PopulationUsed){p_PopulationUsed = PopulationUsed;}
    void changePopulationUsed(int changeValue){p_PopulationUsed += changeValue;}
    //一些便利方法
    bool isDead() const { return p_Hp <= 0; }
    bool canDeploy() const { return p_PopulationUsed < p_PopulationCap; }
};
#endif // PLAYER_H
