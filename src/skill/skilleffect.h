#ifndef SKILLEFFECT_H
#define SKILLEFFECT_H

enum class EffectType {
    AttackSpeedSlow,   // 攻击减速（敌方 debuff）
    AttackSpeedUp,     // 攻击加速（自身 buff）
    MoveSpeedUp,       // 移动加速（自身 buff）
    DamageOverTime,    // 持续扣血（流血/中毒）
    HealOverTime,      // 持续回血
    DamageTakenUp,     // 受到伤害增加（敌方 debuff）
    Invincible,        // 无敌（自身 buff，免疫伤害）
    Sealed,            // 封印（无法行动、无法被攻击）
    Sleeping           // 沉睡（无法行动、可被攻击）
};

struct SkillEffect {
    EffectType type;
    int remainingFrames;  // -1 = 永久（由 Zone 管理生命周期）
    float value;          // 效果值，例如 0.5 = 50%
    void* source;         // 指向 Zone 或 Skill，用于 removeEffectsFromSource
};

#endif // SKILLEFFECT_H
