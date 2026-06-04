#ifndef SKILL_MAGICMIRROR_H
#define SKILL_MAGICMIRROR_H

#include "skill.h"

class SkillMagicMirror : public Skill
{
public:
    SkillMagicMirror();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_MAGICMIRROR_H
