#include "cinderella.h"
#include "skill/skill_crystalshoes.h"
#include "gui/castvisual/crystalshoes_castvisual.h"

cinderella::cinderella()
    : Unit(QStringLiteral("灰姑娘"))
    , m_skill(std::make_unique<CrystalShoesSkill>())
    , m_castVisual(std::make_unique<CrystalShoesCastVisual>(this, m_skill->castFrames()))
{
    //但是我这里不是针对法师，是针对灰姑娘的专属设计
    setMaxHp(250);//法师脆一点，初始的时候
    setMaxMana(60);//这里还有问题，现在蓝条有点少了（但是现在是受限于冷却而不是蓝条）
    setRange(3);//法师手长
    setAtk(8);//普攻伤害要低一点
    setmanaRecoveryAmount(15);//这里还是有问题，现在蓝涨得快又容易触发，后续升级应该怎么调
    setAttackSpeed(20);//法师普攻比较慢
    setMoveSpeed(100);//走地也慢
}

void cinderella::decideState()
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

void cinderella::tickCooldowns(Game* game)
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

void cinderella::resetToDefault()
{
    Unit::resetToDefault();
    m_skill->reset();
    m_castVisual->reset();
}
