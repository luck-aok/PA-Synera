#include "skill_sleepingbeauty.h"
#include "entity/unit.h"
#include "skill/skilleffect.h"

SkillSleepingBeauty::SkillSleepingBeauty()
    : Skill(QStringLiteral("百年沉睡"), 50, 30, 600)
{}

void SkillSleepingBeauty::execute(Unit* caster, Game* /*game*/)
{
    if (!caster || caster->isDead()) return;

    Unit* target = caster->target();
    if (!target || target->isDead()) return;

    // 单体沉睡 180 帧 (3秒)：无法行动，可被攻击
    target->addEffect(SkillEffect{
        EffectType::Sleeping,
        180,
        0,
        nullptr
    });
}
