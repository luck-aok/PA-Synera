#ifndef SKILL_CRYSTALSHOES_H
#define SKILL_CRYSTALSHOES_H

#include "skill.h"

class CrystalShoesSkill : public Skill
{
public:
    CrystalShoesSkill();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_CRYSTALSHOES_H
