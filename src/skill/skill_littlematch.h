#ifndef SKILL_LITTLEMATCH_H
#define SKILL_LITTLEMATCH_H

#include "skill.h"

class SkillLittleMatch : public Skill
{
public:
    SkillLittleMatch();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_LITTLEMATCH_H
