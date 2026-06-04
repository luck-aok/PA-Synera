#include "sleepingbeauty.h"
#include "skill/skill_sleepingbeauty.h"
#include "gui/castvisual/sleepingbeauty_castvisual.h"

SleepingBeauty::SleepingBeauty()
    : Unit(QStringLiteral("睡美人"))
    , m_skill(std::make_unique<SkillSleepingBeauty>())
    , m_castVisual(std::make_unique<SleepingBeautyCastVisual>(this, m_skill->castFrames()))
{
    // 远程控制：中HP中蓝
    setMaxHp(400);
    setHp(400);
    setMaxMana(50);
    setRange(3);                          /* 远程 */
    setAtk(10);
    setmanaRecoveryAmount(10);
    setAttackSpeed(25);
    setMoveSpeed(90);
}

void SleepingBeauty::decideState()
{
    if (state() == State::Dead) return;

    if (m_skill->isCasting()) return;

    if (mana() >= m_skill->manaCost() && m_skill->isReady()) {
        m_castVisual->reset();
        m_skill->startCast();
        setState(State::Casting);
        return;
    }

    Unit::decideState();
}

void SleepingBeauty::tickCooldowns(Game* game)
{
    tickEffects();

    if (!m_skill->isCasting())
        m_skill->Cast_tickCooldown();

    if (state() == State::Casting) {
        m_castVisual->tick();
        m_skill->tickCast();
        if (!m_skill->isCasting()) {
            m_skill->execute(this, game);
            m_skill->startCooldown();
            setMana(0);
            setState(State::Idle);
        }
        return;
    }

    Unit::tickCooldowns(game);
}

void SleepingBeauty::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
