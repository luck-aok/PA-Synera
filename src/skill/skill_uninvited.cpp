#include "skill_uninvited.h"
#include "entity/unit.h"
#include "skill/skilleffect.h"

SkillUninvited::SkillUninvited()
    : Skill(QStringLiteral("空白结局"), 70, 40, 600)
{}

void SkillUninvited::execute(Unit* caster, Game* /*game*/)
{
    if (!caster || caster->isDead()) return;

    Unit* target = caster->target();
    if (!target || target->isDead()) return;

    // 强力诅咒 DOT：每帧扣 12 血，持续 240 帧 (4秒)
    target->addEffect(SkillEffect{
        EffectType::DamageOverTime,
        240,
        12.0f,
        nullptr
    });
}
