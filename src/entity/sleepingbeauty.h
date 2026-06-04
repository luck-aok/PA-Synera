#ifndef SLEEPINGBEAUTY_H
#define SLEEPINGBEAUTY_H

#include "unit.h"
#include "skill/skill.h"
#include "gui/castvisual/castvisual.h"
#include <memory>

class SleepingBeauty : public Unit
{
public:
    SleepingBeauty();

    void decideState() override;
    void tickCooldowns(Game* game) override;
    void resetToDefault() override;

    Skill* skill() const { return m_skill.get(); }
    CastVisual* castVisual() override { return m_castVisual.get(); }

private:
    std::unique_ptr<Skill> m_skill;
    std::unique_ptr<CastVisual> m_castVisual;
};

#endif // SLEEPINGBEAUTY_H
