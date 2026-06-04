#ifndef UNIT_H
#define UNIT_H

#include <QPoint>
#include <QString>
//新加
#include <QStringList>
#include <QList>
#include "skill/skilleffect.h"

class Projectile;
class BoardGeometry;
class Game;
class CastVisual;

class Unit
{
public:
    enum class Owner{
        PlayerCtrl,
        EnemyCtrl
    };
    enum class State{
        Idle,// 空闲，需要寻找目标
        Moving,// 向目标移动中
        Attacking,// 站桩攻击中
        Casting,// 释放技能中
        Dead// 死亡
    };
    explicit Unit(const QString& name = QString("Unit"));
    virtual ~Unit();    // 改成 virtual：子类析构 + 清理飞弹

    int id() const { return m_id; }
    QString name() const { return m_name; }
    QPoint position() const { return m_position; }

    void setName(const QString& name) { m_name = name; }
    void setPosition(const QPoint& pos) { m_position = pos; }
    //
    int atk() const {return m_atk; }
    void setAtk(int new_ATK){m_atk=new_ATK;}
    int hp() const {return m_hp; }
    void setHp(int new_hp){m_hp=new_hp>m_maxHp?m_maxHp:new_hp;}
    int mana() const {return m_mana; }
    void setMana(int new_Mana){m_mana=new_Mana>m_maxMana ? m_maxMana : new_Mana;}
    int range() const {return m_range; }
    void setRange(int new_Range){m_range=new_Range;}
    int maxHp() const {return m_maxHp;}
    void setMaxHp(int new_max_hp){m_maxHp = new_max_hp;}
    int  maxMana() const {return m_maxMana;}
    void setMaxMana(int new_maxMana){m_maxMana = new_maxMana;}
    Owner owner() const { return m_owner; }
    void setOwner(Owner owner) { m_owner = owner; }
    QStringList traits() const { return m_traits; }
    void setTraits(const QStringList& traits) { m_traits = traits; }
    void addTrait(const QString& trait) {
        if (!m_traits.contains(trait))
            m_traits.append(trait);
    }
    int manaRecoveryAmount(){return m_manaRecoveryAmount;}
    void setmanaRecoveryAmount(int n_recoveryAmount){m_manaRecoveryAmount = n_recoveryAmount;}
    bool isDead(){return m_hp <= 0;}

    //updateTarget 目前只是用来更新target
    virtual void updateTarget(const QList<Unit*>& allUnits);

    //状态机
    virtual void tickCooldowns(class Game* game = nullptr);
    virtual void decideState();
    virtual void resetToDefault();

private:
    static int s_nextId;

    int m_id;// 唯一编号（自动递增）
    QString m_name;// 名字，如 "弓手"
    QPoint m_position;// 棋盘坐标 (col, row)
    //PA 要求以后往里面加 HP、攻击力、法力值等
    int m_hp=300;//不同派生类要设定不同值
    int m_atk=10;
    int m_range=1;// 攻击距离
    int m_mana=0;
    int m_maxHp=300;
    int m_maxMana=60;
    int m_manaRecoveryAmount=10;
    Owner m_owner;
    QStringList m_traits;
    int m_starLevel=1;

//////////////////////////////////
//有限状态机
    State m_state = State::Idle;
    int m_attackSpeed = 15;//每多少帧攻击一次
    int m_moveSpeed = 90;//每多少帧移动一格
    int m_attackCooldown = 0;//攻击冷却剩余帧数
    int m_moveCooldown = 0;//移动冷却剩余帧数
    int m_baseAttackSpeed = -1;  // 初始攻速（-1 = 未记录）
    int m_baseMoveSpeed = -1;    // 初始移速（-1 = 未记录）

public:
    //getter
    State state() const {return m_state;}
    int attackSpeed() const {return m_attackSpeed;}
    int moveSpeed() const {return m_moveSpeed;}
    int attackCooldown() const {return m_attackCooldown;}
    int moveCooldown() const{return m_moveCooldown;}

    //setter
    void setState(State new_s){m_state = new_s;}
    //我发现如果要实现减速（攻击或者移动），不仅要改变，还要记住改变前的值
    //这个记住逻辑我不在这写了，要不然会让代码复杂度变高，反而容易因为耦合太多东西而出错
    void setAttackSpeed(int new_attackSpeed){m_attackSpeed = new_attackSpeed;}
    void changeAttackSpeed(int changeamount){m_attackSpeed += changeamount;}
    void setMoveSpeed(int new_moveSpeed){m_moveSpeed = new_moveSpeed;}
    void changeMoveSpeed(int changeamount){m_moveSpeed += changeamount;}
    void setAttackCooldown(int n_attackCooldown){m_attackCooldown = n_attackCooldown;}
    void setMoveCooldown(int n_moveCooldown){m_moveCooldown = n_moveCooldown;}
    void settarget(Unit* new_target){m_target = new_target;}

///////////////////////////////////
//寻路逻辑，不同角色具体实现自己的不同逻辑
private:
    Unit* m_target = nullptr;
    QVector<QPoint> m_path;

public:
    Unit* target() const {return m_target;}
    virtual Unit* findTarget(const QList<Unit*>& allUnits);
    // 用引用返回（QVector<QPoint>&），外部可以直接修改路径，不用 setter。
    QVector<QPoint>& path(){return m_path;}
    //一个便利函数，判断是否在攻击范围内
    bool isInRangeOf(Unit* other)const{
        int dx =qAbs(m_position.x() - other->position().x());
        int dy =qAbs(m_position.y() - other->position().y());
        return dx+dy <= m_range;
    }

/////////////////////////////////////
//攻击发射子弹
public:
    // 投射物
    void addProjectile(Projectile* p) { m_projectiles.append(p); }
    void updateProjectiles();          // 每帧调：tick → 到达的扣血 + delete
    void clearProjectiles();           // 清理所有飞弹（reset 时用）
    virtual Projectile* createProjectile(Unit* target, BoardGeometry* geo);

    virtual CastVisual* castVisual() { return nullptr; }

    // 效果系统
    void addEffect(const SkillEffect& e);
    void removeEffectsFromSource(void* source);
    void tickEffects();

private:
    QList<SkillEffect> m_activeEffects;
    QList<Projectile*> m_projectiles;


};
#endif // UNIT_H
