#ifndef SKILL_H
#define SKILL_H

#include <QString>

class Unit;
class Game;

class Skill
{
public:
    Skill(const QString& name, int manaCost, int castFrames, int cooldownFrames);
    virtual ~Skill() = default;

    QString name() const { return m_name; }
    int manaCost() const { return m_manaCost; }
    int castFrames() const { return m_castFrames; }

    // 施法
    void startCast();
    void tickCast();
    bool isCasting() const;

    // 冷却
    bool isReady() const;
    void startCooldown();
    void Cast_tickCooldown();

    void reset();

    virtual void execute(Unit* caster, Game* game) = 0;

private:
    QString m_name;
    int m_manaCost;
    int m_castFrames;
    int m_castFramesRemaining = 0;
    int m_cooldownFrames;
    int m_cooldownRemaining = 0;
};

#endif // SKILL_H
