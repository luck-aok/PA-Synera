#include "mermaid.h"
#include "skill/skill_mermaid.h"
#include "gui/castvisual/mermaid_castvisual.h"

Mermaid::Mermaid()
    : Unit(QStringLiteral("人鱼"))
    , m_skill(std::make_unique<SkillMermaid>())
    , m_castVisual(std::make_unique<MermaidCastVisual>(this, m_skill->castFrames()))
{
    // 远程法师：中HP中攻，长读条大招
    setMaxHp(280);
    setHp(280);
    setMaxMana(60);
    setRange(3);                          /* 远程 */
    setAtk(10);
    setmanaRecoveryAmount(10);
    setAttackSpeed(25);
    setMoveSpeed(90);
}

void Mermaid::decideState()
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

void Mermaid::tickCooldowns(Game* game)
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

void Mermaid::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
