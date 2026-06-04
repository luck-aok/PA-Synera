#ifndef SKILL_SLEEPINGBEAUTY_H
#define SKILL_SLEEPINGBEAUTY_H

#include "skill.h"

class SkillSleepingBeauty : public Skill
{
public:
    SkillSleepingBeauty();

    void execute(Unit* caster, Game* game) override;
};

#endif // SKILL_SLEEPINGBEAUTY_H
