#include "bench.h"
Bench::Bench():m_slots(Slots,nullptr){}
void Bench::addUnit(Unit* unit, int slot){
    if(!unit || slot<0||slot>=Slots||m_slots[slot]){
        return;
    }
    m_slots[slot] = unit;
    m_unitToSlot[unit] = slot;
    unit->setPosition(QPoint(slot, -1));
}
void Bench::removeUnit(Unit* unit){
    if(!unit || !m_unitToSlot.contains(unit)){
        return;
    }
    int slot = m_unitToSlot.value(unit);
    m_slots[slot] = nullptr;
    m_unitToSlot.remove(unit);
}
Unit* Bench::getUnitAt(int slot) const{
    return (slot>=0 &&slot<Slots) ? m_slots[slot]:nullptr;
}
bool Bench::isSlotEmpty(int slot) const{
    return getUnitAt(slot) == nullptr;
}
bool Bench::isfull() const
{
    for (int i = 0; i < Slots; ++i) {
        if (!m_slots[i]) return false;
    }
    return true;
}
int Bench::slotOf(Unit* unit) const
{
    return m_unitToSlot.value(unit, -1);
}
void Bench::clear()
{
    std::fill(m_slots.begin(), m_slots.end(), nullptr);
    m_unitToSlot.clear();
}




















