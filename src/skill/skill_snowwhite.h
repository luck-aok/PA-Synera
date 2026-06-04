#ifndef SKILL_SNOWWHITE_H
#define SKILL_SNOWWHITE_H

#include "skill.h"

class SkillSnowWhite : public Skill
{
public:
    SkillSnowWhite();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_SNOWWHITE_H
