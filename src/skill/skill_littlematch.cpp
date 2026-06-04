#include "skill_littlematch.h"
#include "entity/unit.h"
#include "core/game.h"

SkillLittleMatch::SkillLittleMatch()
    : Skill(QStringLiteral("最后一把火柴"), 80, 50, 600)
{}

void SkillLittleMatch::execute(Unit* caster, Game* game)
{
    if (!caster || caster->isDead()) return;

    // 对全体敌方造成 120 伤害
    for (Unit* u : game->allUnits()) {
        if (!u || u == caster || u->isDead()) continue;
        if (u->position().y() == -1) continue;
        if (u->owner() == caster->owner()) continue;

        u->setHp(u->hp() - 120);
        if (u->isDead())
            u->setState(Unit::State::Dead);
    }
}
