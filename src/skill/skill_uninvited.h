#ifndef SKILL_UNINVITED_H
#define SKILL_UNINVITED_H

#include "skill.h"

class SkillUninvited : public Skill
{
public:
    SkillUninvited();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_UNINVITED_H
