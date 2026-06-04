#ifndef SKILL_SILVERMANE_H
#define SKILL_SILVERMANE_H

#include "skill.h"

class SkillSilvermane : public Skill
{
public:
    SkillSilvermane();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_SILVERMANE_H
