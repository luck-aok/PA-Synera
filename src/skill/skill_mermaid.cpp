#include "skill_mermaid.h"
#include "entity/unit.h"
#include "skill/skilleffect.h"
#include "core/game.h"
#include <QtMath>

SkillMermaid::SkillMermaid()
    : Skill(QStringLiteral("泡沫化身"), 60, 40, 600)
{}

void SkillMermaid::execute(Unit* caster, Game* game)
{
    if (!caster || caster->isDead()) return;

    // 自身无敌，持续 120 帧 (2秒)
    caster->addEffect(SkillEffect{
        EffectType::Invincible,
        120,
        0,
        nullptr
    });

    // 对周围 2 格内所有敌人造成 100 伤害
    QPoint casterPos = caster->position();
    for (Unit* u : game->allUnits()) {
        if (!u || u == caster || u->isDead()) continue;
        if (u->position().y() == -1) continue;
        if (u->owner() == caster->owner()) continue;

        int dx = qAbs(casterPos.x() - u->position().x());
        int dy = qAbs(casterPos.y() - u->position().y());
        if (dx + dy <= 2) {
            u->setHp(u->hp() - 100);
            if (u->isDead())
                u->setState(Unit::State::Dead);
        }
    }
}
