#ifndef SKILL_MERMAID_H
#define SKILL_MERMAID_H

#include "skill.h"

class SkillMermaid : public Skill
{
public:
    SkillMermaid();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_MERMAID_H
